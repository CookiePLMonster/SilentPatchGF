#pragma once
#include "DirectDraw7Surface_RwD3D9.h"

class DirectDraw7Surface_RwD3D9RT final : public DirectDraw7Surface_RwD3D9
{
public:
	DirectDraw7Surface_RwD3D9RT( DD7_RwD3D9OverlayRenderQueue* renderQueue )
		: m_overlayRenderQueue( renderQueue )
	{
	}

	void AddOverlayToQueue( void* shader, void* raster, const RECT& srcRect, const RECT& destRect )
	{
		m_overlayRenderQueue->PushToQueue( shader, raster, srcRect, destRect );
	}

	void RemoveOverlayFromQueue( void* raster )
	{
		m_overlayRenderQueue->RemoveFromQueue( raster );
	}

private:
	DD7_RwD3D9OverlayRenderQueue* m_overlayRenderQueue;
};

