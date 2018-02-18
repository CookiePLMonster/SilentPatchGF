#define _MEMORY_DECLS_ONLY
#include "MemoryMgr.h"

#include <rwcore.h>

#define RwEngineInstance (*rwengine)
extern void** rwengine;

WRAPPER RwBool RwD3D9CreatePixelShader(const RwUInt32 *function, void **shader) { EAXJMP(0x755C20); }
WRAPPER RwRaster* RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags) { EAXJMP(0x75E6D0); }
WRAPPER RwRaster* RwD3D9RasterCreate(RwUInt32 width, RwUInt32 height, RwUInt32 d3dFormat, RwUInt32 flags) { EAXJMP(0x766FE0); }

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