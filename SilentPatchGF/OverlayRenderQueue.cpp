#include "DirectDraw7_RwD3D9.h"

#include <rwcore.h>
#include "MemoryMgr.h"

#define RwEngineInstance (*rwengine)
extern void** rwengine;

void DD7_RwD3D9OverlayRenderQueue::Render( void* camera )
{
	if ( m_queue.empty() ) return;

	RwCamera* rwCamera =  static_cast<RwCamera*>(camera);
	if ( RwCameraBeginUpdate( rwCamera ) )
	{
		for ( const auto& entry : m_queue )
		{
			RwRenderStateSet( rwRENDERSTATETEXTURERASTER, entry.first );
			RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, static_cast<RwIm2DVertex*>(entry.second), 5);
		}

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
}