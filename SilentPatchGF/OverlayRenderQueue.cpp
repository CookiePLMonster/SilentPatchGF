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
		RwD3D9SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
		RwD3D9SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );

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
			vertices[index].u = static_cast<RwReal>(entry.srcRect.right);
			vertices[index].v = 0.0f;
			vertices[index].emissiveColor = 0xFFFFFFFF;
			index++;

			vertices[index].x = entry.destRect.right - 0.5f;
			vertices[index].y = entry.destRect.bottom - 0.5f;
			vertices[index].z = 0.0f;
			vertices[index].rhw = 0.0f;
			vertices[index].u = static_cast<RwReal>(entry.srcRect.right);
			vertices[index].v = 1.0f;
			vertices[index].emissiveColor = 0xFFFFFFFF;

			const float psData[4] = { 1.0f/entry.srcRect.right };

			im2dShaderOverride = entry.shader;

			_rwD3D9SetPixelShaderConstant( 0, psData, 1 );
			RwRenderStateSet( rwRENDERSTATETEXTURERASTER, entry.raster );
			RwIm2DRenderPrimitive(rwPRIMTYPETRISTRIP, vertices, sizeof(vertices) / sizeof(vertices[0]));
		}

		im2dShaderOverride = nullptr;
		RwCameraEndUpdate( rwCamera );

		m_queue.clear();
	}
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