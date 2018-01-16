#include "DirectDraw7Stub.h"

#include <cassert>

#include <cassert>

#pragma comment(lib, "d3d9.lib")

DirectDraw7Stub::DirectDraw7Stub()
{
	m_direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	assert( m_direct3d != nullptr );

	//D3DPRESENT_PARAMETERS parameters;


	//m_direct3dDevice = m_direct3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetActiveWindow(), D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_FPU_PRESERVE )
}


HRESULT DirectDraw7Stub::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return E_NOTIMPL;
}

ULONG DirectDraw7Stub::AddRef(void)
{
	return 0;
}

ULONG DirectDraw7Stub::Release(void)
{
	return 0;
}

HRESULT DirectDraw7Stub::Compact(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::CreateClipper(DWORD, LPDIRECTDRAWCLIPPER *, IUnknown *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::CreatePalette(DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE *, IUnknown *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::CreateSurface(LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPDIRECTDRAWSURFACE7 * lplpDDSurface, IUnknown * pUnkOuter)
{
	IDirectDrawSurface7* surface = new DirectDraw7SurfaceStub();
	*reinterpret_cast<IDirectDrawSurface7**>(lplpDDSurface) = surface;

	return DD_OK;
}

HRESULT DirectDraw7Stub::DuplicateSurface(LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7 *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::EnumDisplayModes(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::EnumSurfaces(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::FlipToGDISurface(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps)
{
	lpDDDriverCaps->dwCaps |= DDCAPS_OVERLAY|DDCAPS_OVERLAYFOURCC;
	return DD_OK;
}

HRESULT DirectDraw7Stub::GetDisplayMode(LPDDSURFACEDESC2 lpDDSurfaceDesc2)
{
	if ( lpDDSurfaceDesc2 == nullptr || lpDDSurfaceDesc2->dwSize != sizeof(*lpDDSurfaceDesc2) ) return DDERR_INVALIDPARAMS;

	D3DDISPLAYMODE displayMode;
	HRESULT result = m_direct3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &displayMode );
	if ( result != D3D_OK ) return DDERR_INVALIDPARAMS;

	lpDDSurfaceDesc2->dwWidth = displayMode.Width;
	lpDDSurfaceDesc2->dwHeight = displayMode.Height;
	lpDDSurfaceDesc2->dwRefreshRate = displayMode.RefreshRate;
	lpDDSurfaceDesc2->dwFlags = DDSD_WIDTH|DDSD_HEIGHT|DDSD_REFRESHRATE;

	return DD_OK;
}

HRESULT DirectDraw7Stub::GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes)
{
	*lpNumCodes = 1;
	lpCodes[0] = D3DFMT_YUY2;
	return DD_OK;
}

HRESULT DirectDraw7Stub::GetGDISurface(LPDIRECTDRAWSURFACE7 *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::GetMonitorFrequency(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::GetScanLine(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::GetVerticalBlankStatus(LPBOOL)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::Initialize(GUID *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::RestoreDisplayMode(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
{
	// We don't want to allow the app to go fullscreen here - but allow it to act like a normal application
	// using a fallback code the game has
	return dwFlags == DDSCL_NORMAL ? DD_OK : DDERR_EXCLUSIVEMODEALREADYSET;
}

HRESULT DirectDraw7Stub::SetDisplayMode(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::WaitForVerticalBlank(DWORD, HANDLE)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::GetAvailableVidMem(LPDDSCAPS2, LPDWORD, LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::GetSurfaceFromDC(HDC, LPDIRECTDRAWSURFACE7 *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::RestoreAllSurfaces(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::TestCooperativeLevel(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::StartModeTest(LPSIZE, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Stub::EvaluateMode(DWORD, DWORD *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return E_NOTIMPL;
}

ULONG DirectDraw7SurfaceStub::AddRef(void)
{
	return 0;
}

ULONG DirectDraw7SurfaceStub::Release(void)
{
	return 0;
}

HRESULT DirectDraw7SurfaceStub::AddAttachedSurface(LPDIRECTDRAWSURFACE7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::AddOverlayDirtyRect(LPRECT)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::Blt(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::BltBatch(LPDDBLTBATCH, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE7, LPRECT, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::Flip(LPDIRECTDRAWSURFACE7, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetAttachedSurface(LPDDSCAPS2, LPDIRECTDRAWSURFACE7 *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetBltStatus(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetCaps(LPDDSCAPS2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetClipper(LPDIRECTDRAWCLIPPER *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetColorKey(DWORD, LPDDCOLORKEY)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetDC(HDC *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetFlipStatus(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetOverlayPosition(LPLONG, LPLONG)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetPalette(LPDIRECTDRAWPALETTE *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetPixelFormat(LPDDPIXELFORMAT)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetSurfaceDesc(LPDDSURFACEDESC2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::Initialize(LPDIRECTDRAW, LPDDSURFACEDESC2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::IsLost(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::Lock(LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::ReleaseDC(HDC)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::Restore(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::SetClipper(LPDIRECTDRAWCLIPPER)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::SetColorKey(DWORD, LPDDCOLORKEY)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::SetOverlayPosition(LONG, LONG)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::SetPalette(LPDIRECTDRAWPALETTE)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::Unlock(LPRECT)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::UpdateOverlayDisplay(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetDDInterface(LPVOID *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::PageLock(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::PageUnlock(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::SetSurfaceDesc(LPDDSURFACEDESC2, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::SetPrivateData(REFGUID, LPVOID, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetPrivateData(REFGUID, LPVOID, LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::FreePrivateData(REFGUID)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetUniquenessValue(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::ChangeUniquenessValue(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::SetPriority(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetPriority(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::SetLOD(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7SurfaceStub::GetLOD(LPDWORD)
{
	return E_NOTIMPL;
}
