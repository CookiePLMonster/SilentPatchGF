#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#define WINVER 0x0502
#define _WIN32_WINNT 0x0502

#include <windows.h>
#include "MemoryMgr.h"

#include "DirectDraw7_RwD3D9.h"
#include <cassert>

#include <rwcore.h>

extern RwCamera    *RwCameraShowRaster_DrawOverlay(RwCamera * camera, void *pDev, RwUInt32 flags);

HRESULT WINAPI DirectDrawRwD3D9Create( GUID *lpGUID, LPVOID *lplpDD, REFIID iid, IUnknown *pUnkOuter )
{
	IDirectDraw7* stub = new DirectDraw7_RwD3D9();
	*reinterpret_cast<IDirectDraw7**>(lplpDD) = stub;
	return DD_OK;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);

	switch ( reason )
	{
	case DLL_PROCESS_ATTACH:
		{
			ScopedUnprotect::Section Section( GetModuleHandle( nullptr ), ".text");
			ScopedUnprotect::Section Section2( GetModuleHandle( nullptr ), ".rdata");
			strcpy_s( (char*)0xA8F4D8, 16, "log.log" );

			Memory::InjectHook( 0x8C58D6, DirectDrawRwD3D9Create );

			InstallRenderQueueHook();
			//Memory::InjectHook( 0x75AE90, RwCameraShowRaster_DrawOverlay, PATCH_JUMP );

			break;
		}
	}

	return TRUE;
}