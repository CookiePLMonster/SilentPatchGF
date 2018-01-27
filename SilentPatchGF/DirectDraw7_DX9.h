#pragma once

#include <ddraw.h>
#include <d3d9.h>

class DirectDraw7_DX9 final : public IDirectDraw7
{
public:
	// Inherited via IDirectDraw7
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID * ppvObj) override;
	virtual ULONG WINAPI AddRef(void) override;
	virtual ULONG WINAPI Release(void) override;
	virtual HRESULT WINAPI Compact(void) override;					 
	virtual HRESULT WINAPI CreateClipper(DWORD, LPDIRECTDRAWCLIPPER *, IUnknown *) override;					 
	virtual HRESULT WINAPI CreatePalette(DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE *, IUnknown *) override;					 
	virtual HRESULT WINAPI CreateSurface(LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPDIRECTDRAWSURFACE7 * lplpDDSurface, IUnknown * pUnkOuter) override;					 
	virtual HRESULT WINAPI DuplicateSurface(LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7 *) override;					 
	virtual HRESULT WINAPI EnumDisplayModes(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2) override;					 
	virtual HRESULT WINAPI EnumSurfaces(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK7) override;					 
	virtual HRESULT WINAPI FlipToGDISurface(void) override;					 
	virtual HRESULT WINAPI GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps) override;					 
	virtual HRESULT WINAPI GetDisplayMode(LPDDSURFACEDESC2) override;					 
	virtual HRESULT WINAPI GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes) override;					 
	virtual HRESULT WINAPI GetGDISurface(LPDIRECTDRAWSURFACE7 *) override;					 
	virtual HRESULT WINAPI GetMonitorFrequency(LPDWORD) override;			 
	virtual HRESULT WINAPI GetScanLine(LPDWORD) override;				 
	virtual HRESULT WINAPI GetVerticalBlankStatus(LPBOOL) override;				 
	virtual HRESULT WINAPI Initialize(GUID *) override;				 
	virtual HRESULT WINAPI RestoreDisplayMode(void) override;				 
	virtual HRESULT WINAPI SetCooperativeLevel(HWND hWnd, DWORD dwFlags) override;				 
	virtual HRESULT WINAPI SetDisplayMode(DWORD, DWORD, DWORD, DWORD, DWORD) override;		 
	virtual HRESULT WINAPI WaitForVerticalBlank(DWORD, HANDLE) override;				 
	virtual HRESULT WINAPI GetAvailableVidMem(LPDDSCAPS2, LPDWORD, LPDWORD) override;				 
	virtual HRESULT WINAPI GetSurfaceFromDC(HDC, LPDIRECTDRAWSURFACE7 *) override;				 
	virtual HRESULT WINAPI RestoreAllSurfaces(void) override;				 
	virtual HRESULT WINAPI TestCooperativeLevel(void) override;				 
	virtual HRESULT WINAPI GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2, DWORD) override;				 
	virtual HRESULT WINAPI StartModeTest(LPSIZE, DWORD, DWORD) override;
	virtual HRESULT WINAPI EvaluateMode(DWORD, DWORD *) override;

	DirectDraw7_DX9();

private:
	const D3DDISPLAYMODE& CollectDisplayMode();

private:
	LONG m_refCount = 1;
	IDirect3D9* m_d3d = nullptr;

	bool m_adapterDisplayModeGathered = false;
	D3DDISPLAYMODE m_adapterDisplayMode;
};

