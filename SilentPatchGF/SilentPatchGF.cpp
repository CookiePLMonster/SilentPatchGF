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

	if ( int INIoption = GetPrivateProfileIntW(L"SilentPatch", L"FPSLimit", -1, wcModulePath); INIoption != -1 )
	{
		Memory::Patch<float>(0x7CF398 + 6, INIoption > 0 ? 1000.0f/INIoption : 0.0f );
	}
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

			

			InstallRenderQueueHook();
			//Memory::InjectHook( 0x75AE90, RwCameraShowRaster_DrawOverlay, PATCH_JUMP );

			break;
		}
	}

	return TRUE;
}