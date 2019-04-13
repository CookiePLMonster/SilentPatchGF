#pragma once

#include <cstdint>

struct RwCamera;

enum RwRasterFormat
{
	rwRASTERFORMAT8888 = 0x0500,    /**<32 bits - 8 bits per component */
};

enum RwRasterType
{
	rwRASTERTYPETEXTURE = 0x04,         /**<Texture */
};

enum RwRasterLockMode
{
	rwRASTERLOCKWRITE = 0x01,   /**<Lock for writing */
	rwRASTERLOCKREAD = 0x02,    /**<Lock for reading */
	rwRASTERLOCKNOFETCH = 0x04, /**<When used in combination with
								*  rwRASTERLOCKWRITE, asks the driver not to
								*  fetch the pixel data. This is only useful
								*  if it is known that ALL the raster data is
								*  going to be overwritten before the raster
								*  is unlocked, i.e. from an 
								*  \ref RwRasterSetFromImage call. This flag
								*  is not supported by all drivers. */
};

struct RwRaster
{
	RwRaster           *parent; /* Top level raster if a sub raster */
	uint8_t            *cpPixels; /* Pixel pointer when locked */
	uint8_t            *palette; /* Raster palette */
	int32_t             width, height, depth; /* Dimensions of raster */
	int32_t             stride; /* Lines bytes of raster */
	int16_t             nOffsetX, nOffsetY; /* Sub raster offset */
	uint8_t             cType;  /* Type of raster */
	uint8_t             cFlags; /* Raster flags */
	uint8_t             privateFlags; /* Raster private flags */
	uint8_t             cFormat; /* Raster format */

	uint8_t            *originalPixels;
	int32_t             originalWidth;
	int32_t             originalHeight;
	int32_t             originalStride;
};

inline int32_t RwRasterGetWidth(const RwRaster *raster)
{
	return raster->width;
}

inline int32_t RwRasterGetHeight(const RwRaster *raster)
{
	return raster->height;
}

inline int32_t RwRasterGetStride(const RwRaster *raster)
{
	return raster->stride;
}

struct RwVideoMode
{
	int32_t         width;   /**< Width  */
	int32_t         height;  /**< Height */
	int32_t         depth;   /**< Depth  */
	int32_t         flags;   /**< Flags  */
	int32_t         refRate; /**< Approximate refresh rate */
	int32_t         format;  /**< Raster format
							 * \see RwRasterFormat 
							 */
};

struct RwIm2DVertex
{
	float      x;              /**< Screen X */
	float      y;              /**< Screen Y */
	float      z;              /**< Screen Z */
	float      rhw;            /**< Reciprocal of homogeneous W */

	uint32_t    emissiveColor;  /**< Vertex color */

	float      u;              /**< Texture coordinate U */
	float      v;              /**< Texture coordinate V */
};

enum RwRenderState
{
	rwRENDERSTATENARENDERSTATE = 0,

	rwRENDERSTATETEXTURERASTER,
	/**<Raster used for texturing (normally used in immediate mode). 
	*  The value is a pointer to an \ref RwRaster.
	* Default: NULL.
	*/
};

enum RwPrimitiveType
{
	rwPRIMTYPETRISTRIP = 4,     /**<Connected triangles sharing an edge with, at most, one
								* other forming a series (for example, 3 triangles specified
								* as 0-2-1, 1-2-3-, 2-4-3) */
};

RwCamera* RwCameraBeginUpdate(RwCamera* camera);
RwCamera* RwCameraEndUpdate(RwCamera* camera);
RwCamera* RwCameraShowRaster(RwCamera* camera, void* dev, int32_t flags);

RwRaster* RwRasterCreate(int32_t width, int32_t height, int32_t depth, int32_t flags);
int RwRasterDestroy(RwRaster* raster);
uint8_t* RwRasterLock(RwRaster* raster, uint8_t level, int32_t lockMode);
RwRaster* RwRasterUnlock(RwRaster* raster);

int RwD3D9CreatePixelShader(const uint32_t *function, void **shader);
void RwD3D9DeletePixelShader(void *shader);

void __rwD3D9RenderStateReset();

RwVideoMode* RwEngineGetVideoModeInfo(RwVideoMode* modeinfo, int32_t modeIndex);
int32_t RwEngineGetCurrentVideoMode(void);

int RwRenderStateSet(RwRenderState state, void *value);
int RwIm2DRenderPrimitive(RwPrimitiveType primType, RwIm2DVertex *vertices, int32_t numVertices);

#define RwEngineInstance (*rwengine)
extern void** rwengine;

static_assert(sizeof(RwRaster) == 0x34, "Wrong size: RwRaster");
static_assert(sizeof(RwVideoMode) == 0x18, "Wrong size: RwVideoMode");
static_assert(sizeof(RwIm2DVertex) == 0x1C, "Wrong size: RwIm2DVertex");