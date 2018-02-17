#pragma once
#include "DirectDraw7_RwD3D9.h"


class DirectDraw7Surface_RwD3D9 : public IDirectDrawSurface7
{
public:
	// Inherited via IDirectDrawSurface7
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID * ppvObj) override;
	virtual ULONG WINAPI AddRef(void) override;
	virtual ULONG WINAPI Release(void) override;
	virtual HRESULT WINAPI AddAttachedSurface(LPDIRECTDRAWSURFACE7) override;
	virtual HRESULT WINAPI AddOverlayDirtyRect(LPRECT) override;
	virtual HRESULT WINAPI Blt(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX) override;
	virtual HRESULT WINAPI BltBatch(LPDDBLTBATCH, DWORD, DWORD) override;
	virtual HRESULT WINAPI BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE7, LPRECT, DWORD) override;
	virtual HRESULT WINAPI DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE7) override;
	virtual HRESULT WINAPI EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK7) override;
	virtual HRESULT WINAPI EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK7) override;
	virtual HRESULT WINAPI Flip(LPDIRECTDRAWSURFACE7, DWORD) override;
	virtual HRESULT WINAPI GetAttachedSurface(LPDDSCAPS2, LPDIRECTDRAWSURFACE7 *) override;
	virtual HRESULT WINAPI GetBltStatus(DWORD) override;
	virtual HRESULT WINAPI GetCaps(LPDDSCAPS2) override;
	virtual HRESULT WINAPI GetClipper(LPDIRECTDRAWCLIPPER *) override;
	virtual HRESULT WINAPI GetColorKey(DWORD, LPDDCOLORKEY) override;
	virtual HRESULT WINAPI GetDC(HDC *) override;
	virtual HRESULT WINAPI GetFlipStatus(DWORD) override;
	virtual HRESULT WINAPI GetOverlayPosition(LPLONG, LPLONG) override;
	virtual HRESULT WINAPI GetPalette(LPDIRECTDRAWPALETTE *) override;
	virtual HRESULT WINAPI GetPixelFormat(LPDDPIXELFORMAT) override;
	virtual HRESULT WINAPI GetSurfaceDesc(LPDDSURFACEDESC2) override;
	virtual HRESULT WINAPI Initialize(LPDIRECTDRAW, LPDDSURFACEDESC2) override;
	virtual HRESULT WINAPI IsLost(void) override;
	virtual HRESULT WINAPI Lock(LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE) override;
	virtual HRESULT WINAPI ReleaseDC(HDC) override;
	virtual HRESULT WINAPI Restore(void) override;
	virtual HRESULT WINAPI SetClipper(LPDIRECTDRAWCLIPPER) override;
	virtual HRESULT WINAPI SetColorKey(DWORD, LPDDCOLORKEY) override;
	virtual HRESULT WINAPI SetOverlayPosition(LONG, LONG) override;
	virtual HRESULT WINAPI SetPalette(LPDIRECTDRAWPALETTE) override;
	virtual HRESULT WINAPI Unlock(LPRECT) override;
	virtual HRESULT WINAPI UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX) override;
	virtual HRESULT WINAPI UpdateOverlayDisplay(DWORD) override;
	virtual HRESULT WINAPI UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE7) override;
	virtual HRESULT WINAPI GetDDInterface(LPVOID *) override;
	virtual HRESULT WINAPI PageLock(DWORD) override;
	virtual HRESULT WINAPI PageUnlock(DWORD) override;
	virtual HRESULT WINAPI SetSurfaceDesc(LPDDSURFACEDESC2, DWORD) override;
	virtual HRESULT WINAPI SetPrivateData(REFGUID, LPVOID, DWORD, DWORD) override;
	virtual HRESULT WINAPI GetPrivateData(REFGUID, LPVOID, LPDWORD) override;
	virtual HRESULT WINAPI FreePrivateData(REFGUID) override;
	virtual HRESULT WINAPI GetUniquenessValue(LPDWORD) override;
	virtual HRESULT WINAPI ChangeUniquenessValue(void) override;
	virtual HRESULT WINAPI SetPriority(DWORD) override;
	virtual HRESULT WINAPI GetPriority(LPDWORD) override;
	virtual HRESULT WINAPI SetLOD(DWORD) override;
	virtual HRESULT WINAPI GetLOD(LPDWORD) override;

	virtual ~DirectDraw7Surface_RwD3D9() = default;

private:
	LONG m_refCount = 1;
};