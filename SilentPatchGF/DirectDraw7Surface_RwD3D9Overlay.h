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

	DirectDraw7Surface_RwD3D9Overlay( void* shader, int32_t width, int32_t height );
	virtual ~DirectDraw7Surface_RwD3D9Overlay() override;

private:
	void			DecodeYUY2ToYUV();

	void*			m_shader = nullptr;
	RwRaster*		m_raster = nullptr;
	uint8_t*		m_rasterPixelsPtr = nullptr;
	uint8_t*		m_rasterYUY2Ptr = nullptr;
	uint8_t*		m_rasterEndPtr = nullptr;
};

