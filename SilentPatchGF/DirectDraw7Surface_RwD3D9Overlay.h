#pragma once
#include "DirectDraw7Surface_RwD3D9.h"

#include <rwcore.h>
#include <cstdint>

class DirectDraw7Surface_RwD3D9Overlay final : public DirectDraw7Surface_RwD3D9
{
public:
	virtual HRESULT WINAPI Lock(LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE) override;
	virtual HRESULT WINAPI Unlock(LPRECT) override;
	virtual HRESULT WINAPI UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX) override;

	DirectDraw7Surface_RwD3D9Overlay( int32_t width, int32_t height );
	virtual ~DirectDraw7Surface_RwD3D9Overlay() override;

private:
	RwRaster*		m_raster = nullptr;

	RwIm2DVertex	m_verts[5];
};

