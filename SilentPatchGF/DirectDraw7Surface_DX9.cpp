#include "DirectDraw7Surface_DX9.h"


HRESULT DirectDraw7Surface_DX9::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return E_NOTIMPL;
}

ULONG DirectDraw7Surface_DX9::AddRef(void)
{
	return 0;
}

ULONG DirectDraw7Surface_DX9::Release(void)
{
	return 0;
}

HRESULT DirectDraw7Surface_DX9::AddAttachedSurface(LPDIRECTDRAWSURFACE7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::AddOverlayDirtyRect(LPRECT)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::Blt(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::BltBatch(LPDDBLTBATCH, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE7, LPRECT, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::Flip(LPDIRECTDRAWSURFACE7, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetAttachedSurface(LPDDSCAPS2, LPDIRECTDRAWSURFACE7 *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetBltStatus(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetCaps(LPDDSCAPS2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetClipper(LPDIRECTDRAWCLIPPER *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetColorKey(DWORD, LPDDCOLORKEY)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetDC(HDC *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetFlipStatus(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetOverlayPosition(LPLONG, LPLONG)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetPalette(LPDIRECTDRAWPALETTE *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetPixelFormat(LPDDPIXELFORMAT)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetSurfaceDesc(LPDDSURFACEDESC2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::Initialize(LPDIRECTDRAW, LPDDSURFACEDESC2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::IsLost(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::Lock(LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::ReleaseDC(HDC)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::Restore(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::SetClipper(LPDIRECTDRAWCLIPPER)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::SetColorKey(DWORD, LPDDCOLORKEY)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::SetOverlayPosition(LONG, LONG)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::SetPalette(LPDIRECTDRAWPALETTE)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::Unlock(LPRECT)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::UpdateOverlayDisplay(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetDDInterface(LPVOID *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::PageLock(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::PageUnlock(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::SetSurfaceDesc(LPDDSURFACEDESC2, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::SetPrivateData(REFGUID, LPVOID, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetPrivateData(REFGUID, LPVOID, LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::FreePrivateData(REFGUID)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetUniquenessValue(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::ChangeUniquenessValue(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::SetPriority(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetPriority(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::SetLOD(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_DX9::GetLOD(LPDWORD)
{
	return E_NOTIMPL;
}
