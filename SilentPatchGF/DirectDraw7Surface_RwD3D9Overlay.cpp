#include "DirectDraw7Surface_RwD3D9Overlay.h"
#include "DirectDraw7Surface_RwD3D9RT.h"

#include <cassert>

static int32_t DD7LockModeToRWLockMode( DWORD dwFlags )
{
	int32_t lockMode = 0;
	if ( (dwFlags & DDLOCK_READONLY) ) lockMode = rwRASTERLOCKREAD;
	else if ( (dwFlags & DDLOCK_WRITEONLY) ) lockMode = rwRASTERLOCKWRITE|rwRASTERLOCKNOFETCH;

	return lockMode;
}

HRESULT DirectDraw7Surface_RwD3D9Overlay::Lock(LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
{
	if ( lpDestRect != nullptr || lpDDSurfaceDesc == nullptr ) return DDERR_INVALIDPARAMS;

	uint8_t* pixels = RwRasterLock( m_raster, 0, DD7LockModeToRWLockMode( dwFlags ) );
	lpDDSurfaceDesc->lpSurface = pixels;

	return pixels != nullptr ? DD_OK : DDERR_OUTOFMEMORY;
}

HRESULT DirectDraw7Surface_RwD3D9Overlay::Unlock(LPRECT lpRect)
{
	if ( lpRect != nullptr ) return DDERR_INVALIDRECT;

	return RwRasterUnlock( m_raster ) != nullptr ? DD_OK : DDERR_NOTLOCKED;
}

HRESULT DirectDraw7Surface_RwD3D9Overlay::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE7 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
	if ( (dwFlags & DDOVER_SHOW) != 0 )
	{
		DirectDraw7Surface_RwD3D9RT* rt = static_cast<DirectDraw7Surface_RwD3D9RT*>(lpDDDestSurface);
		rt->AddOverlayToQueue( m_raster, *lpSrcRect, *lpDestRect );
		return DD_OK;
	}
	else if ( (dwFlags & DDOVER_HIDE) != 0 )
	{
		// Do nothing
		return DD_OK;
	}

	assert( !"Unimplemented UpdateOverlay flag!" );
	return E_NOTIMPL;
}

DirectDraw7Surface_RwD3D9Overlay::DirectDraw7Surface_RwD3D9Overlay(int32_t width, int32_t height)
{
	m_raster = RwRasterCreate( width / 2, height, 0, rwRASTERFORMAT8888|rwRASTERTYPETEXTURE );
}

DirectDraw7Surface_RwD3D9Overlay::~DirectDraw7Surface_RwD3D9Overlay()
{
	// TODO: Raster destroy
}
