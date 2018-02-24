#include "MemoryMgr.h"
#include "Patterns.h"

#include <rwcore.h>
#include <objbase.h>

#define RwEngineInstance (*rwengine)
void** rwengine = *hook::get_pattern<void**>( "74 75 8B 15", 4 );

static void* varRwD3D9CreatePixelShader = Memory::ReadCallFrom( hook::get_pattern( "E8 ? ? ? ? 83 C4 30 FF 05" ) );
WRAPPER RwBool RwD3D9CreatePixelShader(const RwUInt32 *function, void **shader) { VARJMP(varRwD3D9CreatePixelShader); }

static void* varRwRasterCreate = hook::get_pattern( "8B 0D ? ? ? ? 8B 54 01 60 56 68 07 04 03 00 52", -5 );
WRAPPER RwRaster* RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags) { VARJMP(varRwRasterCreate); }
static void* varRwRasterDestroy = hook::get_pattern( "6A 00 56 6A 00 FF 50 5C", -0x15 );
WRAPPER RwBool RwRasterDestroy(RwRaster * raster) { VARJMP(varRwRasterDestroy); }
static void* varRwRasterShowRaster = hook::get_pattern( "56 8B B0 98 00 00 00 57 E8", -5 );
WRAPPER RwRaster* RwRasterShowRaster(RwRaster * raster, void *dev, RwUInt32 flags) { VARJMP(varRwRasterShowRaster); }

static void* varRwEngineGetVideoModeInfo = hook::get_pattern( "83 EC 08 56 8B 74 24 10 50 6A 00 56 6A 06 FF 51 14", -0xA );
WRAPPER RwVideoMode *RwEngineGetVideoModeInfo(RwVideoMode *modeinfo, RwInt32 modeIndex) { VARJMP(varRwEngineGetVideoModeInfo); }
static void* varRwEngineGetCurrentVideoMode =hook::get_pattern( "83 EC 0C 6A 00 6A 00 8D 44 24 08 50 6A 0A FF 51 14 ", -0x6 );
WRAPPER RwInt32 RwEngineGetCurrentVideoMode(void) { VARJMP(varRwEngineGetCurrentVideoMode); }

void RwD3D9DeletePixelShader(void *shader)
{
	static_cast<IUnknown*>(shader)->Release();
}

RwCamera *
RwCameraShowRaster(RwCamera *camera, void *dev, RwUInt32 flags)
{
	if (RwRasterShowRaster(camera->frameBuffer, dev, flags))
	{
		return camera;
	}

	/* Device error */
	return NULL;
}

RwCamera    *RwCameraBeginUpdate(RwCamera * camera)
{
	return camera->beginUpdate(camera);
}

RwCamera    *RwCameraEndUpdate(RwCamera * camera)
{
	return camera->endUpdate(camera);
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