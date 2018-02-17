#include "DirectDraw7Surface_DX9Overlay.h"
#include "DirectDraw7Surface_DX9Device.h"

#include <cassert>

DirectDraw7Surface_DX9Overlay::DirectDraw7Surface_DX9Overlay( DirectDraw7_DX9* proxy, IDirect3DDevice9Ex* device, UINT Width, UINT Height )
	: DirectDraw7Surface_DX9( proxy, proxy->GetD3D() )
{
	HRESULT hr = device->CreateOffscreenPlainSurface( Width, Height, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &m_d3dSurface, nullptr );
	assert( SUCCEEDED(hr) );
}

HRESULT DirectDraw7Surface_DX9Overlay::Lock( LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent )
{
	// TODO: Error checking
	m_directDrawProxy->BeginSceneWrapper();

	D3DLOCKED_RECT lockedRect;
	HRESULT hr = m_d3dSurface->LockRect( &lockedRect, lpDestRect, D3DLOCK_DISCARD );
	assert( SUCCEEDED(hr) );
	if ( SUCCEEDED(hr) )
	{
		lpDDSurfaceDesc->lpSurface = lockedRect.pBits;
	}
	return hr;
}

HRESULT DirectDraw7Surface_DX9Overlay::Unlock(LPRECT)
{
	m_directDrawProxy->BeginSceneWrapper();

	return m_d3dSurface->UnlockRect();
}

HRESULT DirectDraw7Surface_DX9Overlay::UpdateOverlay( LPRECT lpSrcRect, LPDIRECTDRAWSURFACE7 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx )
{
	// TODO: QueryInterface
	DirectDraw7Surface_DX9Device* deviceSurface = reinterpret_cast<DirectDraw7Surface_DX9Device*>(lpDDDestSurface);
	return deviceSurface->PresentOverlay( this, lpSrcRect, lpDestRect, dwFlags );
}
