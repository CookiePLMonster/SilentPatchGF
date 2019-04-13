#include "DirectDraw7_RwD3D9.h"
#include "DirectDraw7Surface_RwD3D9Overlay.h"
#include "DirectDraw7Surface_RwD3D9RT.h"

#include <d3d9.h>

DD7_RwD3D9OverlayRenderQueue DirectDraw7_RwD3D9::ms_overlayRenderQueue;

HRESULT DirectDraw7_RwD3D9::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	if ( ppvObj == nullptr ) return E_POINTER;

	if ( riid == __uuidof(IUnknown) ||
		riid == IID_IDirectDraw7 )
	{
		*ppvObj = static_cast<IDirectDraw7*>(this);
		AddRef();
		return S_OK;
	}

	*ppvObj = nullptr;
	return E_NOINTERFACE;
}

ULONG DirectDraw7_RwD3D9::AddRef(void)
{
	return InterlockedIncrement( &m_refCount );
}

ULONG DirectDraw7_RwD3D9::Release(void)
{
	ULONG ref = InterlockedDecrement( &m_refCount );
	if ( ref == 0 )
	{
		delete this;
	}
	return ref;
}

HRESULT DirectDraw7_RwD3D9::Compact(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::CreateClipper(DWORD, LPDIRECTDRAWCLIPPER *, IUnknown *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::CreatePalette(DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE *, IUnknown *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::CreateSurface(LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPDIRECTDRAWSURFACE7 * lplpDDSurface, IUnknown * pUnkOuter)
{
	if ( lpDDSurfaceDesc2 == nullptr || lplpDDSurface == nullptr || pUnkOuter != nullptr ) return DDERR_INVALIDPARAMS;

	*lplpDDSurface = nullptr;
	if ( (lpDDSurfaceDesc2->dwFlags & DDSD_CAPS) == 0 ) return DDERR_INVALIDPARAMS;

	if ( (lpDDSurfaceDesc2->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) != 0 )
	{
		// For primary surface, we create an empty wrapper (only pushing overlays to render queue)
		IDirectDrawSurface7* surface = new DirectDraw7Surface_RwD3D9RT( &ms_overlayRenderQueue );
		*lplpDDSurface = surface;
	}
	else if ( (lpDDSurfaceDesc2->ddsCaps.dwCaps & DDSCAPS_OVERLAY) != 0 ) // If it's not an overlay, something went wrong
	{
		// For overlay surface, we create a primitive to be rendered
		IDirectDrawSurface7* surface = new DirectDraw7Surface_RwD3D9Overlay( m_yuy2Shader, lpDDSurfaceDesc2->dwWidth, lpDDSurfaceDesc2->dwHeight );
		*lplpDDSurface = surface;
	}
	else
	{
		return DDERR_INVALIDPARAMS;
	}

	return DD_OK;
}

HRESULT DirectDraw7_RwD3D9::DuplicateSurface(LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7 *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::EnumDisplayModes(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::EnumSurfaces(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::FlipToGDISurface(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps)
{
	lpDDDriverCaps->dwCaps |= DDCAPS_OVERLAY|DDCAPS_OVERLAYFOURCC;
	return DD_OK;
}

HRESULT DirectDraw7_RwD3D9::GetDisplayMode(LPDDSURFACEDESC2 lpDDSurfaceDesc2)
{
	if ( lpDDSurfaceDesc2 == nullptr || lpDDSurfaceDesc2->dwSize != sizeof(*lpDDSurfaceDesc2) ) return DDERR_INVALIDPARAMS;

	RwVideoMode modeinfo;
	RwEngineGetVideoModeInfo( &modeinfo, RwEngineGetCurrentVideoMode() );

	lpDDSurfaceDesc2->dwWidth = modeinfo.width;
	lpDDSurfaceDesc2->dwHeight = modeinfo.height;
	lpDDSurfaceDesc2->dwDepth = modeinfo.depth;
	lpDDSurfaceDesc2->dwRefreshRate = modeinfo.refRate;
	lpDDSurfaceDesc2->dwFlags = DDSD_WIDTH|DDSD_HEIGHT|DDSD_DEPTH|DDSD_REFRESHRATE;

	return DD_OK;
}

HRESULT DirectDraw7_RwD3D9::GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes)
{
	*lpNumCodes = 1;
	lpCodes[0] = D3DFMT_YUY2;
	return DD_OK;
}

HRESULT DirectDraw7_RwD3D9::GetGDISurface(LPDIRECTDRAWSURFACE7 *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::GetMonitorFrequency(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::GetScanLine(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::GetVerticalBlankStatus(LPBOOL)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::Initialize(GUID *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::RestoreDisplayMode(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
{
	// We don't want to allow the app to go fullscreen here - but allow it to act like a normal application
	// using a fallback code the game has
	return dwFlags == DDSCL_NORMAL ? DD_OK : DDERR_EXCLUSIVEMODEALREADYSET;
}

HRESULT DirectDraw7_RwD3D9::SetDisplayMode(DWORD, DWORD, DWORD, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::WaitForVerticalBlank(DWORD, HANDLE)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::GetAvailableVidMem(LPDDSCAPS2, LPDWORD, LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::GetSurfaceFromDC(HDC, LPDIRECTDRAWSURFACE7 *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::RestoreAllSurfaces(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::TestCooperativeLevel(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::StartModeTest(LPSIZE, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7_RwD3D9::EvaluateMode(DWORD, DWORD *)
{
	return E_NOTIMPL;
}

DirectDraw7_RwD3D9::~DirectDraw7_RwD3D9()
{
	if ( m_yuy2Shader != nullptr )
	{
		RwD3D9DeletePixelShader( m_yuy2Shader );
	}
}

#include "resource.h"

extern HINSTANCE hDLLModule;
void DirectDraw7_RwD3D9::CreateYUY2Shader()
{
	HRSRC		resource = FindResource(hDLLModule, MAKEINTRESOURCE(IDR_YUY2SHADER), RT_RCDATA);
	uint32_t*	shader = static_cast<uint32_t*>(LockResource( LoadResource(hDLLModule, resource) ));

	RwD3D9CreatePixelShader(shader, reinterpret_cast<void**>(&m_yuy2Shader));
}
