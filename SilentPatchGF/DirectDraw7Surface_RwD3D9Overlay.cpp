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
	if ( pixels != nullptr )
	{
		m_rasterPixelsPtr = pixels;
		lpDDSurfaceDesc->lpSurface = m_rasterYUY2Ptr = pixels + ((RwRasterGetStride( m_raster ) * RwRasterGetHeight( m_raster )) / 2);
		m_rasterEndPtr = pixels + ( RwRasterGetStride( m_raster ) * RwRasterGetHeight( m_raster ) );
		return DD_OK;
	}

	lpDDSurfaceDesc->lpSurface = nullptr;
	return DDERR_OUTOFMEMORY;
}

HRESULT DirectDraw7Surface_RwD3D9Overlay::Unlock(LPRECT lpRect)
{
	if ( lpRect != nullptr ) return DDERR_INVALIDRECT;

	DecodeYUY2ToYUV();
	return RwRasterUnlock( m_raster ) != nullptr ? DD_OK : DDERR_NOTLOCKED;
}

HRESULT DirectDraw7Surface_RwD3D9Overlay::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE7 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
	if ( (dwFlags & DDOVER_SHOW) != 0 )
	{
		DirectDraw7Surface_RwD3D9RT* rt = static_cast<DirectDraw7Surface_RwD3D9RT*>(lpDDDestSurface);
		rt->AddOverlayToQueue( m_shader, m_raster, *lpSrcRect, *lpDestRect );
		return DD_OK;
	}
	else if ( (dwFlags & DDOVER_HIDE) != 0 )
	{
		// If we've already queued for overlay for render, remove it
		DirectDraw7Surface_RwD3D9RT* rt = static_cast<DirectDraw7Surface_RwD3D9RT*>(lpDDDestSurface);
		rt->RemoveOverlayFromQueue( m_raster );
		return DD_OK;
	}

	assert( !"Unimplemented UpdateOverlay flag!" );
	return E_NOTIMPL;
}

DirectDraw7Surface_RwD3D9Overlay::DirectDraw7Surface_RwD3D9Overlay(void* shader, int32_t width, int32_t height)
	: m_shader(shader)
{
	m_raster = RwRasterCreate( width, height, 0, rwRASTERFORMAT8888|rwRASTERTYPETEXTURE );
}

DirectDraw7Surface_RwD3D9Overlay::~DirectDraw7Surface_RwD3D9Overlay()
{
	if ( m_raster != nullptr )
	{
		RwRasterDestroy( m_raster );
	}
}

void DirectDraw7Surface_RwD3D9Overlay::DecodeYUY2ToYUV()
{
	const uint32_t* src = reinterpret_cast<uint32_t*>(m_rasterYUY2Ptr);
	const uint32_t* end = reinterpret_cast<uint32_t*>(m_rasterEndPtr);

	uint32_t* dest = reinterpret_cast<uint32_t*>(m_rasterPixelsPtr);

	while ( src != end )
	{
		const uint32_t yuy = *src++;
		*dest++ = (yuy & ~(0xFF0000)) | ((yuy & 0xFF) << 16);
		*dest++ = yuy;
	}
}
