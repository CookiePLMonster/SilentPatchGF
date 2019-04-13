#include "RW.h"

#include "MemoryMgr.h"
#include "Patterns.h"

#include <objbase.h>

#define rwSTANDARDRASTERLOCK            15  /* Lock a raster to get it's pixels */
#define rwSTANDARDRASTERUNLOCK          16  /* Unlock a raster to return it's pixels */

typedef int (*RwStandardFunc)(void *pOut,void *pInOut,int32_t nI);
typedef RwCamera   *(*RwCameraBeginUpdateFunc) (RwCamera * camera);
typedef RwCamera   *(*RwCameraEndUpdateFunc) (RwCamera * camera);

typedef int
(*RwIm2DRenderPrimitiveFunction)(RwPrimitiveType primType,
	RwIm2DVertex *vertices,
	int32_t numVertices);

typedef int
(*RwRenderStateSetFunction)(RwRenderState nState,void *pParam);

void** rwengine = *hook::get_pattern<void**>( "74 75 8B 15", 4 );

static void* varRwD3D9CreatePixelShader = Memory::ReadCallFrom( hook::get_pattern( "E8 ? ? ? ? 83 C4 30 FF 05" ) );
WRAPPER int RwD3D9CreatePixelShader(const uint32_t *function, void **shader) { VARJMP(varRwD3D9CreatePixelShader); }

static void* var__rwD3D9RenderStateReset = hook::get_pattern( "53 8B 1D ? ? ? ? 55 56 8B 35 ? ? ? ? 57 89 44 24 10", -6 );
WRAPPER void __rwD3D9RenderStateReset() { VARJMP(var__rwD3D9RenderStateReset); }

static void* varRwRasterCreate = hook::get_pattern( "8B 0D ? ? ? ? 8B 54 01 60 56 68 07 04 03 00 52", -5 );
WRAPPER RwRaster* RwRasterCreate(int32_t width, int32_t height, int32_t depth, int32_t flags) { VARJMP(varRwRasterCreate); }
static void* varRwRasterDestroy = hook::get_pattern( "6A 00 56 6A 00 FF 50 5C", -0x15 );
WRAPPER int RwRasterDestroy(RwRaster* raster) { VARJMP(varRwRasterDestroy); }
static void* varRwRasterShowRaster = hook::get_pattern( "56 8B B0 98 00 00 00 57 E8", -5 );
WRAPPER RwRaster* RwRasterShowRaster(RwRaster* raster, void* dev, int32_t flags) { VARJMP(varRwRasterShowRaster); }

static void* varRwEngineGetVideoModeInfo = hook::get_pattern( "83 EC 08 56 8B 74 24 10 50 6A 00 56 6A 06 FF 51 14", -0xA );
WRAPPER RwVideoMode* RwEngineGetVideoModeInfo(RwVideoMode* modeinfo, int32_t modeIndex) { VARJMP(varRwEngineGetVideoModeInfo); }
static void* varRwEngineGetCurrentVideoMode =hook::get_pattern( "83 EC 0C 6A 00 6A 00 8D 44 24 08 50 6A 0A FF 51 14 ", -0x6 );
WRAPPER int32_t RwEngineGetCurrentVideoMode(void) { VARJMP(varRwEngineGetCurrentVideoMode); }

static RwRaster* rwCameraGetFrameBuffer( RwCamera* camera )
{
	const uintptr_t cam = reinterpret_cast<uintptr_t>(camera);
	return *(RwRaster**)( cam + 0x60 );
}

static RwCameraBeginUpdateFunc rwCameraGetBeginUpdateFunc( RwCamera* camera )
{
	const uintptr_t cam = reinterpret_cast<uintptr_t>(camera);
	return *(RwCameraBeginUpdateFunc*)( cam + 0x18 );
}

static RwCameraEndUpdateFunc rwCameraGetEndUpdateFunc( RwCamera* camera )
{
	const uintptr_t cam = reinterpret_cast<uintptr_t>(camera);
	return *(RwCameraBeginUpdateFunc*)( cam + 0x1C );
}

static RwStandardFunc* rwEngineGetStandardFuncs()
{
	const uintptr_t engine = reinterpret_cast<uintptr_t>( RwEngineInstance );
	return (RwStandardFunc*)( engine + 0x48 );
}

static RwIm2DRenderPrimitiveFunction rwEngineGetRenderPrimitiveFunction()
{
	const uintptr_t engine = reinterpret_cast<uintptr_t>( RwEngineInstance );
	return *(RwIm2DRenderPrimitiveFunction*)( engine + 0x30 );
}

static RwRenderStateSetFunction rwEngineGetRenderStateSetFunction()
{
	const uintptr_t engine = reinterpret_cast<uintptr_t>( RwEngineInstance );
	return *(RwRenderStateSetFunction*)( engine + 0x20 );
}


void RwD3D9DeletePixelShader(void *shader)
{
	static_cast<IUnknown*>(shader)->Release();
}

RwCamera* RwCameraShowRaster(RwCamera* camera, void* dev, int32_t flags)
{
	if (RwRasterShowRaster(rwCameraGetFrameBuffer(camera), dev, flags))
	{
		return camera;
	}

	/* Device error */
	return NULL;
}

RwCamera* RwCameraBeginUpdate(RwCamera* camera)
{
	RwCameraBeginUpdateFunc func = rwCameraGetBeginUpdateFunc(camera);
	return func(camera);
}

RwCamera* RwCameraEndUpdate(RwCamera* camera)
{
	RwCameraEndUpdateFunc func = rwCameraGetEndUpdateFunc(camera);
	return func(camera);
}


uint8_t* RwRasterLock(RwRaster* raster, uint8_t level, int32_t lockMode)
{
	uint8_t* pixelPtr;

	if (rwEngineGetStandardFuncs() [rwSTANDARDRASTERLOCK]
		((void *) &pixelPtr, (void *) raster,
		((int32_t) lockMode) + (((int32_t) level) << 8)))
	{
		return pixelPtr;
	}

	return NULL;
}

RwRaster* RwRasterUnlock(RwRaster* raster)
{
	rwEngineGetStandardFuncs() [rwSTANDARDRASTERUNLOCK] (NULL, (void *) raster,
		0);
	
	return raster;
}

int RwRenderStateSet(RwRenderState state, void *value)
{
	RwRenderStateSetFunction func = rwEngineGetRenderStateSetFunction();
	return func(state, value);
}

int RwIm2DRenderPrimitive(RwPrimitiveType primType, RwIm2DVertex *vertices, int32_t numVertices)
{
	RwIm2DRenderPrimitiveFunction func = rwEngineGetRenderPrimitiveFunction();
	return func(primType, vertices, numVertices);
}