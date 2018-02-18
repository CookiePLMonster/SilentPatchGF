#include "DirectDraw7_RwD3D9.h"

#include "resource.h"
#include <rwcore.h>
#include "MemoryMgr.h"

#define RwEngineInstance (*rwengine)
extern void** rwengine;

extern HINSTANCE hDLLModule;

void (*org_rwD3D9SetPixelShader)(void* shader);

static void* im2dShaderOverride = nullptr;
void _rwD3D9SetPixelShader_Override( void* )
{
	org_rwD3D9SetPixelShader( im2dShaderOverride );
}

void DD7_RwD3D9OverlayRenderQueue::Render( void* camera )
{
	if ( m_queue.empty() ) return;

	// TODO: Move elsewhere (probably to DirectDraw7_RwD3D9 constructor)
	static void* yuy2Shader = nullptr;
	static bool shaderCreated = false;
	if ( !shaderCreated )
	{
		shaderCreated = true;

		// TODO: This should be a bit different
		HRSRC		resource = FindResource(hDLLModule, MAKEINTRESOURCE(IDR_YUY2SHADER), RT_RCDATA);
		RwUInt32*	shader = static_cast<RwUInt32*>(LockResource( LoadResource(hDLLModule, resource) ));

		RwD3D9CreatePixelShader(shader, reinterpret_cast<void**>(&yuy2Shader));
	}

	RwCamera* rwCamera =  static_cast<RwCamera*>(camera);
	if ( RwCameraBeginUpdate( rwCamera ) )
	{
		im2dShaderOverride = yuy2Shader;

		for ( const auto& entry : m_queue )
		{
			RwRenderStateSet( rwRENDERSTATETEXTURERASTER, entry.first );
			RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, static_cast<RwIm2DVertex*>(entry.second), 5);
		}

		im2dShaderOverride = nullptr;
		RwCameraEndUpdate( rwCamera );

		m_queue.clear();
	}
}



/*		RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, vertices, index);

		RwCameraEndUpdate( camera );
	}
}*/

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