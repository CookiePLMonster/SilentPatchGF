#pragma once
#include "DirectDraw7Surface_DX9.h"

class DirectDraw7Surface_DX9Overlay final : public DirectDraw7Surface_DX9
{
public:
	DirectDraw7Surface_DX9Overlay( DirectDraw7_DX9* proxy, IDirect3DDevice9Ex* device, UINT Width, UINT Height );

	virtual HRESULT WINAPI Lock(LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE) override;
	virtual HRESULT WINAPI Unlock(LPRECT) override;
	virtual HRESULT WINAPI UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX) override;

	IDirect3DSurface9* GetD3DSurface() const { return m_d3dSurface; }

	virtual ~DirectDraw7Surface_DX9Overlay() override
	{
		if ( m_d3dSurface != nullptr )
		{
			m_d3dSurface->Release();
		}
	}

private:
	IDirect3DSurface9* m_d3dSurface = nullptr;
};

