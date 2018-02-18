#define _MEMORY_DECLS_ONLY
#include "MemoryMgr.h"

#include <rwcore.h>

#define RwEngineInstance (*rwengine)
extern void** rwengine;

WRAPPER void _rwD3D9SetPixelShaderConstant(RwUInt32 registerAddress, const void *constantData, RwUInt32 constantCount) { EAXJMP(0x755C60); }
WRAPPER RwBool RwD3D9CreatePixelShader(const RwUInt32 *function, void **shader) { EAXJMP(0x755C20); }
WRAPPER RwRaster* RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags) { EAXJMP(0x75E6D0); }
WRAPPER void RwD3D9SetSamplerState(RwUInt32 stage, RwUInt32 type, RwUInt32 value) { EAXJMP(0x75B460); }

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