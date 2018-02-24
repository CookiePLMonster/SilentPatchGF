#include "DirectDraw7Surface_RwD3D9.h"

#include <rwcore.h>
#define RwEngineInstance (*rwengine)

void** rwengine = (void**)0x0102EAB8;


HRESULT DirectDraw7Surface_RwD3D9::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	if ( ppvObj == nullptr ) return E_POINTER;

	if ( riid == __uuidof(IUnknown) ||
		riid == IID_IDirectDrawSurface7 )
	{
		*ppvObj = static_cast<IDirectDrawSurface7*>(this);
		AddRef();
		return S_OK;
	}

	*ppvObj = nullptr;
	return E_POINTER;
}

ULONG DirectDraw7Surface_RwD3D9::AddRef(void)
{
	return _InterlockedIncrement( &m_refCount );
}

ULONG DirectDraw7Surface_RwD3D9::Release(void)
{
	LONG ref = _InterlockedDecrement( &m_refCount );
	if ( ref == 0 )
	{
		delete this;
	}
	return ref;
}

HRESULT DirectDraw7Surface_RwD3D9::AddAttachedSurface(LPDIRECTDRAWSURFACE7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::AddOverlayDirtyRect(LPRECT)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::Blt(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::BltBatch(LPDDBLTBATCH, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE7, LPRECT, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::Flip(LPDIRECTDRAWSURFACE7, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetAttachedSurface(LPDDSCAPS2, LPDIRECTDRAWSURFACE7 *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetBltStatus(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetCaps(LPDDSCAPS2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetClipper(LPDIRECTDRAWCLIPPER *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetColorKey(DWORD, LPDDCOLORKEY)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetDC(HDC *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetFlipStatus(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetOverlayPosition(LPLONG, LPLONG)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetPalette(LPDIRECTDRAWPALETTE *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetPixelFormat(LPDDPIXELFORMAT)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetSurfaceDesc(LPDDSURFACEDESC2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::Initialize(LPDIRECTDRAW, LPDDSURFACEDESC2)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::IsLost(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::Lock(LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::ReleaseDC(HDC)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::Restore(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::SetClipper(LPDIRECTDRAWCLIPPER)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::SetColorKey(DWORD, LPDDCOLORKEY)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::SetOverlayPosition(LONG, LONG)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::SetPalette(LPDIRECTDRAWPALETTE)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::Unlock(LPRECT)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::UpdateOverlayDisplay(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE7)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetDDInterface(LPVOID *)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::PageLock(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::PageUnlock(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::SetSurfaceDesc(LPDDSURFACEDESC2, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::SetPrivateData(REFGUID, LPVOID, DWORD, DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetPrivateData(REFGUID, LPVOID, LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::FreePrivateData(REFGUID)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetUniquenessValue(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::ChangeUniquenessValue(void)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::SetPriority(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetPriority(LPDWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::SetLOD(DWORD)
{
	return E_NOTIMPL;
}

HRESULT DirectDraw7Surface_RwD3D9::GetLOD(LPDWORD)
{
	return E_NOTIMPL;
}

RwCamera    *RwCameraBeginUpdate(RwCamera * camera)
{
	return camera->beginUpdate(camera);
}

RwCamera    *RwCameraEndUpdate(RwCamera * camera)
{
	return camera->endUpdate(camera);
}

#include "MemoryMgr.h"

WRAPPER RwRaster    *RwRasterShowRaster(RwRaster * raster, void *dev, RwUInt32 flags) { EAXJMP(0x75E640); }

RwCamera *
RwCameraShowRaster(RwCamera *camera, void *dev, RwUInt32 flags)
{
	if (RwRasterShowRaster(camera->frameBuffer, dev, flags))
	{
		return camera;
	}

	/* Device error */
	return NULL;
}

