#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#define WINVER 0x0502
#define _WIN32_WINNT 0x0502

#include <windows.h>
#include "MemoryMgr.h"

#include "DirectDraw7Stub.h"
#include <cassert>

HRESULT WINAPI DirectDrawStubCreate( GUID *lpGUID, LPVOID *lplpDD, REFIID iid, IUnknown *pUnkOuter )
{
	IDirectDraw7* stub = new DirectDraw7Stub();
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

			Memory::InjectHook( 0x8C58D6, DirectDrawStubCreate );

			break;
		}
	}

	return TRUE;
}