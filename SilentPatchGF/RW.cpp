#define _MEMORY_DECLS_ONLY
#include "MemoryMgr.h"

#include <rwcore.h>
#include <objbase.h>

#define RwEngineInstance (*rwengine)
extern void** rwengine;

WRAPPER RwBool RwD3D9CreatePixelShader(const RwUInt32 *function, void **shader) { EAXJMP(0x755C20); }
WRAPPER RwRaster* RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags) { EAXJMP(0x75E6D0); }
WRAPPER RwBool RwRasterDestroy(RwRaster * raster) { EAXJMP(0x75E4D0); }

WRAPPER RwVideoMode *RwEngineGetVideoModeInfo(RwVideoMode *modeinfo, RwInt32 modeIndex) { EAXJMP(0x74BCA0); }
WRAPPER RwInt32 RwEngineGetCurrentVideoMode(void) { EAXJMP(0x74BD00); }

void RwD3D9DeletePixelShader(void *shader)
{
	static_cast<IUnknown*>(shader)->Release();
}

RwUInt8* RwRasterLock(RwRaster* raster, RwUInt8 level, RwInt32 lockMode)
{
	RwUInt8* pixelPtr;

	if (RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERLOCK])
		((void *) &pixelPtr, (void *) raster,
		((RwInt32) lockMode) + (((RwInt32) level) << 8)))
	{
		return pixelPtr;
	}

	return NULL;
}

RwRaster* RwRasterUnlock(RwRaster* raster)
{
	RWSRCGLOBAL(stdFunc[rwSTANDARDRASTERUNLOCK]) (NULL, (void *) raster,
		0);
	
	return raster;
}