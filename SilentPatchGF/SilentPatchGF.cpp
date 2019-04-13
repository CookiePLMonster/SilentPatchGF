#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#define WINVER 0x0502
#define _WIN32_WINNT 0x0502

#include <windows.h>
#include "MemoryMgr.h"
#include "Patterns.h"
#include "LateStaticInit.h"

#include "DirectDraw7_RwD3D9.h"
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

HINSTANCE hDLLModule;

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

	using namespace Memory;
	using namespace hook;

	// Don't rely on Install Dir registry entry for obtaining game path
	{
		static char	gameDirPath[MAX_PATH];
		GetModuleFileNameA(nullptr, gameDirPath, MAX_PATH);

		auto addr1 = pattern( "6A ? 33 DB BE ? ? ? ? 68 ? ? ? ? 8D 4C 24 50" ).get_one();
		uintptr_t addr2 = (uintptr_t)ReadCallFrom( get_pattern( "E8 ? ? ? ? 50 8D 54 24 2C 68" ), 0x20 );

		char* slashPos = strrchr( gameDirPath, '\\' );
		if ( slashPos != nullptr )
		{
			const auto stringSize = std::distance( gameDirPath, slashPos ) + 1;

			Patch<uint8_t>( addr1.get<void>( 1 ), stringSize );
			Patch<const char*>( addr1.get<void>( 9 + 1 ), gameDirPath );

			Patch<uint8_t>( addr2 + 1, stringSize );	
			Patch<const char*>( addr2 + 9 + 1, gameDirPath );
		}

		Patch<uint8_t>( addr1.get<void>( 0x23 + 1 ), 10 );
		Patch<const char*>(addr1.get<void>( 0x25 + 1 ), "_XYZDUMMY_" );

		Patch<uint8_t>( addr2 + 0x2A + 1, 10 );	
		Patch<const char*>( addr2 + 0x2C + 1, "_XYZDUMMY_" );
	}

	if ( const int INIoption = GetPrivateProfileIntW(L"SilentPatch", L"FPSLimit", -1, wcModulePath); INIoption != -1 )
	{
		Patch<float>( get_pattern( "C7 05 ? ? ? ? ? ? ? ? 7D 06", 6 ), INIoption > 0 ? 1000.0f/INIoption : 0.0f );
	}

	if ( GetPrivateProfileIntW(L"SilentPatch", L"SkipIntroMovies", FALSE, wcModulePath) != FALSE )
	{
		void* getUndefinedValue = ReadCallFrom( get_pattern( "E8 ? ? ? ? EB D1") );
		InjectHook( get_pattern( "83 EC 08 56 88 88 91 00 00 00 88 88 92 00 00 00", -6 ), getUndefinedValue, PATCH_JUMP );
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

			static LateStaticInit init( []() {
				const HINSTANCE hInstance = GetModuleHandle( nullptr );
				std::unique_ptr<ScopedUnprotect::Unprotect> Protect = ScopedUnprotect::UnprotectSectionOrFullModule( hInstance, ".text" );
				ScopedUnprotect::Section Section2( hInstance, ".rdata");

				using namespace Memory;
				using namespace hook;

				// Delayed patches
				{
					void* addr = get_pattern( "68 BB 7C 00 00 E8", 5 );
					ReadCall( addr, orgDelayedHookingPoint );
					InjectHook( addr, InjectDelayedPatches );
				}

				// Log file relocated to game directory
				{
					char** str = get_pattern<char*>( "8B 0D ? ? ? ? 89 15 ? ? ? ? 8D 54 24 14", 2 );
					strcpy_s( *str, 16, "log.log" );
				}

				// DirectDraw7 -> RwD3D9 wrapper
				InjectHook( get_pattern( "89 3B E8 ? ? ? ? 3B C7", 2 ), DirectDrawRwD3D9Create );

				// Disallow writing to HKEY_LOCAL_MACHINE
				{
					void* addr = get_pattern( "FF 15 ? ? ? ? 85 C0 75 3D", 2 );

					orgCreateKeyExA = **(decltype(orgCreateKeyExA)**)addr;
					Patch<const void*>( addr, &pRegCreateKeyExA );
				}

				// Default to desktop resolution
				{
					RECT			desktop;
					GetWindowRect(GetDesktopWindow(), &desktop);

					Patch( get_pattern( "64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 EC AC 04 00 00", -7 ), { 0x33, 0xC0, 0xC2, 0x08, 0x00 } ); // xor eax, eax / retn 8
					Patch<int32_t>( get_pattern( "68 ? ? ? ? 50 E8 ? ? ? ? 50 8D 4C 24 58", 1 ), desktop.right );
					Patch<int32_t>( get_pattern( "68 ? ? ? ? 51 E8 ? ? ? ? 50 8D 54 24 3C", 1 ), desktop.bottom );

					auto resolution = pattern( "8D 4C 24 30 E8 ? ? ? ? BE" ).get_one();
					Patch<int32_t>( resolution.get<void>( 9 + 1 ), desktop.right );
					Patch<int32_t>( resolution.get<void>( 14 + 1 ), desktop.bottom );
				}

				InstallRenderQueueHook();
			} );

			LateStaticInit::TryApplyWithPredicate( []() -> bool {
				return hook::pattern( "BF 94 00 00 00 8B C7" ).count_hint(1).size() != 0;
			} );
			break;
		}
	}

	return TRUE;
}

extern "C" __declspec(dllexport)
uint32_t GetBuildNumber()
{
	return (SILENTPATCH_REVISION_ID << 8) | SILENTPATCH_BUILD_ID;
}