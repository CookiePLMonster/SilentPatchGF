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

			const RECT destRect = CalcRectForAR( entry.srcRect, entry.destRect );
			const UVCoords uvCoords = CalcUVForAR( entry.srcRect, entry.destRect );

			size_t index = 0;
			vertices[index].x = destRect.left - 0.5f;
			vertices[index].y = destRect.top - 0.5f;
			vertices[index].z = 0.0f;
			vertices[index].rhw = 0.0f;
			vertices[index].u = uvCoords.left;
			vertices[index].v = uvCoords.top;
			vertices[index].emissiveColor = 0xFFFFFFFF;
			index++;

			vertices[index].x = destRect.left - 0.5f;
			vertices[index].y = destRect.bottom - 0.5f;
			vertices[index].z = 0.0f;
			vertices[index].rhw = 0.0f;
			vertices[index].u = uvCoords.left;
			vertices[index].v = uvCoords.bottom;
			vertices[index].emissiveColor = 0xFFFFFFFF;
			index++;

			vertices[index].x = destRect.right - 0.5f;
			vertices[index].y = destRect.top - 0.5f;
			vertices[index].z = 0.0f;
			vertices[index].rhw = 0.0f;
			vertices[index].u = uvCoords.right;
			vertices[index].v = uvCoords.top;
			vertices[index].emissiveColor = 0xFFFFFFFF;
			index++;

			vertices[index].x = destRect.right - 0.5f;
			vertices[index].y = destRect.bottom - 0.5f;
			vertices[index].z = 0.0f;
			vertices[index].rhw = 0.0f;
			vertices[index].u = uvCoords.right;
			vertices[index].v = uvCoords.bottom;
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

void DD7_RwD3D9OverlayRenderQueue::SetKeepAR(int mode)
{
	if ( mode == 1 )
	{
		m_stretchMode = StretchMode::Letterbox;
	}
	else if ( mode == 2 )
	{
		m_stretchMode = StretchMode::Crop;
	}
	else
	{
		m_stretchMode = StretchMode::Stretch;
	}
}

RECT DD7_RwD3D9OverlayRenderQueue::CalcRectForAR(const RECT& srcRect, const RECT& destRect) const
{
	RECT rect = destRect;
	if ( m_stretchMode == StretchMode::Letterbox )
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
			const LONG outRectHeight = destWidth * srcHeight / srcWidth;
			rect.top = ( destHeight - outRectHeight ) / 2;
			rect.bottom = ( destHeight + outRectHeight ) / 2;
		}
		else if ( srcAR < destAR )
		{
			// Pillarbox
			const LONG outRectWidth = destHeight * srcWidth / srcHeight;
			rect.left = ( destWidth - outRectWidth ) / 2;
			rect.right = ( destWidth + outRectWidth ) / 2;
		}
	}
	return rect;
}

auto DD7_RwD3D9OverlayRenderQueue::CalcUVForAR(const RECT& srcRect, const RECT& destRect) const -> UVCoords
{
	UVCoords rect = { 0.0f, 0.0f, 1.0f, 1.0f };
	if ( m_stretchMode == StretchMode::Crop )
	{
		const LONG srcWidth = srcRect.right - srcRect.left;
		const LONG srcHeight = srcRect.bottom - srcRect.top;
		const double srcAR = static_cast<double>(srcWidth) / srcHeight;

		const LONG destWidth = destRect.right - destRect.left;
		const LONG destHeight = destRect.bottom - destRect.top;
		const double destAR = static_cast<double>(destWidth) / destHeight;
		if ( srcAR > destAR )
		{
			// Crop left/right
			const LONG outSrcWidth = srcHeight * destWidth / destHeight;
			rect.left = static_cast<float>( 1.0 - static_cast<double>(outSrcWidth)/srcWidth ) / 2.0f;
			rect.right = 1.0f - rect.left;
		}
		else if ( srcAR < destAR )
		{
			// Crop top/bottom
			const LONG outSrcHeight = srcWidth * destHeight / destWidth;
			rect.top = static_cast<float>( 1.0 - static_cast<double>(outSrcHeight)/srcHeight ) / 2.0f;
			rect.bottom = 1.0f - rect.top;
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