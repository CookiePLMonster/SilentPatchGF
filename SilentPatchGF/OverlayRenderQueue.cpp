#include "DirectDraw7_RwD3D9.h"

#include <algorithm>
#include <rwcore.h>
#include <d3d9.h>
#include "MemoryMgr.h"

#define RwEngineInstance (*rwengine)
extern void** rwengine;

void (*org_rwD3D9SetPixelShader)(void* shader);

static void* im2dShaderOverride = nullptr;
void _rwD3D9SetPixelShader_Override( void* )
{
	org_rwD3D9SetPixelShader( im2dShaderOverride );
}

void DD7_RwD3D9OverlayRenderQueue::RemoveFromQueue(void* raster)
{
	auto it = std::find_if( m_queue.begin(), m_queue.end(), [=] (const auto& entry) {
		return entry.raster == raster;
	});
	if ( it != m_queue.end() )
	{
		m_queue.erase( it );
		RemoveFromQueue( raster );
	}
}

void DD7_RwD3D9OverlayRenderQueue::Render( void* camera )
{
	if ( m_queue.empty() ) return;

	RwCamera* rwCamera =  static_cast<RwCamera*>(camera);
	if ( RwCameraBeginUpdate( rwCamera ) )
	{
		for ( const auto& entry : m_queue )
		{
			RwIm2DVertex	vertices[4];

			size_t index = 0;
			vertices[index].x = entry.destRect.left - 0.5f;
			vertices[index].y = entry.destRect.top - 0.5f;
			vertices[index].z = 0.0f;
			vertices[index].rhw = 0.0f;
			vertices[index].u = 0.0f;
			vertices[index].v = 0.0f;
			vertices[index].emissiveColor = 0xFFFFFFFF;
			index++;

			vertices[index].x = entry.destRect.left - 0.5f;
			vertices[index].y = entry.destRect.bottom - 0.5f;
			vertices[index].z = 0.0f;
			vertices[index].rhw = 0.0f;
			vertices[index].u = 0.0f;
			vertices[index].v = 1.0f;
			vertices[index].emissiveColor = 0xFFFFFFFF;
			index++;

			vertices[index].x = entry.destRect.right - 0.5f;
			vertices[index].y = entry.destRect.top - 0.5f;
			vertices[index].z = 0.0f;
			vertices[index].rhw = 0.0f;
			vertices[index].u = 1.0f;
			vertices[index].v = 0.0f;
			vertices[index].emissiveColor = 0xFFFFFFFF;
			index++;

			vertices[index].x = entry.destRect.right - 0.5f;
			vertices[index].y = entry.destRect.bottom - 0.5f;
			vertices[index].z = 0.0f;
			vertices[index].rhw = 0.0f;
			vertices[index].u = 1.0f;
			vertices[index].v = 1.0f;
			vertices[index].emissiveColor = 0xFFFFFFFF;
			index++;

			im2dShaderOverride = entry.shader;

			RwRenderStateSet( rwRENDERSTATETEXTURERASTER, entry.raster );
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vertices, index);
		}

		im2dShaderOverride = nullptr;
		RwCameraEndUpdate( rwCamera );

		m_queue.clear();
	}
}

RECT DD7_RwD3D9OverlayRenderQueue::CalcRectForAR(const RECT & srcRect, const RECT & destRect) const
{
	RECT rect = destRect;
	if ( m_keepAspectRatio )
	{
		const LONG srcWidth = srcRect.right - srcRect.left;
		const LONG srcHeight = srcRect.bottom - srcRect.top;
		const double srcAR = static_cast<double>(srcWidth) / srcHeight;

		const LONG destWidth = destRect.right - destRect.left;
		const LONG destHeight = destRect.bottom - destRect.top;
		const double destAR = static_cast<double>(destWidth) / destHeight;
		if ( srcAR > destAR )
		{
			// Letterbox
		}
		else if ( srcAR < destAR )
		{
			// Pillarbox
			const LONG outRectWidth = static_cast<LONG>(destHeight * srcAR);
			rect.left = ( destWidth - outRectWidth ) / 2;
			rect.right = ( destWidth + outRectWidth ) / 2;
		}
	}
	return rect;
}

RwCamera *RwCameraShowRaster_DrawOverlay(RwCamera * camera, void *pDev, RwUInt32 flags)
{
	DirectDraw7_RwD3D9::OverlayRenderQueue().Render( camera );
	return RwCameraShowRaster( camera, pDev, flags );
}

void InstallRenderQueueHook()
{
	Memory::InjectHook( 0x75AE90, RwCameraShowRaster_DrawOverlay, PATCH_JUMP );
	Memory::ReadCall( 0x76F8F4, org_rwD3D9SetPixelShader );
	Memory::InjectHook( 0x76F8F4, _rwD3D9SetPixelShader_Override );
}