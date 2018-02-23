#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#define WINVER 0x0502
#define _WIN32_WINNT 0x0502

#include <windows.h>
#include "MemoryMgr.h"

#include "DirectDraw7_RwD3D9.h"
#include <cassert>
#include <Shlwapi.h>

#include <rwcore.h>

#pragma comment(lib, "shlwapi.lib")

HINSTANCE hDLLModule;

extern RwCamera    *RwCameraShowRaster_DrawOverlay(RwCamera * camera, void *pDev, RwUInt32 flags);

HRESULT WINAPI DirectDrawRwD3D9Create( GUID *lpGUID, LPVOID *lplpDD, REFIID iid, IUnknown *pUnkOuter )
{
	IDirectDraw7* stub = new DirectDraw7_RwD3D9();
	*reinterpret_cast<IDirectDraw7**>(lplpDD) = stub;
	return DD_OK;
}

static LONG (WINAPI *orgCreateKeyExA)( HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition );
LONG WINAPI RegCreateKeyExA_NoHKLM( HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition )
{
	// Game attempts to write to HKEY_LOCAL_MACHINE which triggers registry virtualization
	// so if registry keys are wrong or absent, we'll get a dummy `C:\GFPCROOT\` value as Install Dir
	// and successive launches will use that... nasty and hard to figure out
	if ( hKey == HKEY_LOCAL_MACHINE )
	{
		*phkResult = nullptr;
		return ERROR_INVALID_HANDLE;
	}
	return orgCreateKeyExA( hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition );

}
static auto* const pRegCreateKeyExA = &RegCreateKeyExA_NoHKLM;


static void (__stdcall *orgDelayedHookingPoint)(int);
void __stdcall InjectDelayedPatches( int val )
{
	orgDelayedHookingPoint( val );

	const HINSTANCE hInstance = GetModuleHandle( nullptr );
	std::unique_ptr<ScopedUnprotect::Unprotect> Protect = ScopedUnprotect::UnprotectSectionOrFullModule( hInstance, ".text" );

	// Obtain a path to the ASI
	wchar_t			wcModulePath[MAX_PATH];
	GetModuleFileNameW(hDLLModule, wcModulePath, _countof(wcModulePath) - 3); // Minus max required space for extension
	PathRenameExtensionW(wcModulePath, L".ini");

	// Don't rely on Install Dir registry entry for obtaining game path
	{
		static char	gameDirPath[MAX_PATH];
		GetModuleFileNameA(nullptr, gameDirPath, MAX_PATH);

		char* slashPos = strrchr( gameDirPath, '\\' );
		if ( slashPos != nullptr )
		{
			const auto stringSize = std::distance( gameDirPath, slashPos ) + 1;
			Memory::Patch<uint8_t>( 0x7BEF9C + 1, stringSize );
			Memory::Patch<uint8_t>( 0x7C5300 + 1, stringSize );
			Memory::Patch<const char*>( 0x7BEFA5 + 1, gameDirPath );
			Memory::Patch<const char*>( 0x7C5309 + 1, gameDirPath );
		}

		Memory::Patch<uint8_t>( 0x7BEFBF + 1, 10 );
		Memory::Patch<uint8_t>( 0x7C532A + 1, 10 );
		Memory::Patch<const char*>( 0x7BEFC1 + 1, "_XYZDUMMY_" );
		Memory::Patch<const char*>( 0x7C532C + 1, "_XYZDUMMY_" );
	}

	if ( const int INIoption = GetPrivateProfileIntW(L"SilentPatch", L"FPSLimit", -1, wcModulePath); INIoption != -1 )
	{
		Memory::Patch<float>(0x7CF398 + 6, INIoption > 0 ? 1000.0f/INIoption : 0.0f );
	}

	if ( GetPrivateProfileIntW(L"SilentPatch", L"SkipIntroMovies", FALSE, wcModulePath) != FALSE )
	{
		Memory::Patch<uint32_t>( 0x664AFE + 1, 0x670CA5 );
	}

	int ARmode = GetPrivateProfileIntW(L"SilentPatch", L"KeepFMVAspectRatio", 0, wcModulePath);
	DirectDraw7_RwD3D9::OverlayRenderQueue().SetKeepAR( ARmode );
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);

	switch ( reason )
	{
	case DLL_PROCESS_ATTACH:
		{
			hDLLModule = hModule;

			const HINSTANCE hInstance = GetModuleHandle( nullptr );
			std::unique_ptr<ScopedUnprotect::Unprotect> Protect = ScopedUnprotect::UnprotectSectionOrFullModule( hInstance, ".text" );
			ScopedUnprotect::Section Section2( hInstance, ".rdata");

			strcpy_s( (char*)0xA8F4D8, 16, "log.log" );

			Memory::ReadCall( 0x446B7A, orgDelayedHookingPoint );
			Memory::InjectHook( 0x446B7A, InjectDelayedPatches );

			Memory::InjectHook( 0x8C58D6, DirectDrawRwD3D9Create );

			// Disallow writing to HKEY_LOCAL_MACHINE
			orgCreateKeyExA = **(decltype(orgCreateKeyExA)**)(0x818CD2 + 2);
			Memory::Patch<const void*>( 0x818CD2 + 2, &pRegCreateKeyExA );



			// Default to desktop resolution
			{
				RECT			desktop;
				GetWindowRect(GetDesktopWindow(), &desktop);

				Memory::Patch( 0x8129A0, { 0x33, 0xC0, 0xC2, 0x08, 0x00 } ); // xor eax, eax / retn 8
				Memory::Patch<int32_t>( 0x447122 + 1, desktop.right );
				Memory::Patch<int32_t>( 0x4471C4 + 1, desktop.right );
				Memory::Patch<int32_t>( 0x447188 + 1, desktop.bottom );
				Memory::Patch<int32_t>( 0x4471C9 + 1, desktop.bottom );
			}

			InstallRenderQueueHook();
			break;
		}
	}

	return TRUE;
}