#pragma once

#include <ddraw.h>
#include <vector>

class DD7_RwD3D9OverlayRenderQueue
{
public:
	void PushToQueue( void* shader, void* raster, const RECT& srcRect, const RECT& destRect )
	{
		RemoveFromQueue( raster ); // First remove the overlay if it's already pushed
		m_queue.emplace_back( shader, raster, srcRect, destRect );
	}

	void RemoveFromQueue( void* raster );

	void Render( void* camera );

	void SetKeepAR( int mode );

private:
	enum class StretchMode
	{
		Stretch,
		Letterbox,
		Crop,
	};

	struct UVCoords
	{
		float    left;
		float    top;
		float    right;
		float    bottom;
	};

	RECT CalcRectForAR( const RECT& srcRect, const RECT& destRect ) const;
	UVCoords CalcUVForAR( const RECT& srcRect, const RECT& destRect ) const;

	struct RenderEntry
	{
		void* shader;
		void* raster;
		RECT srcRect;
		RECT destRect;

		RenderEntry( void* shader, void* raster, const RECT& srcRect, const RECT& destRect )
			: shader(shader), raster( raster ), srcRect( srcRect ), destRect( destRect )
		{
		}
	};

	std::vector< RenderEntry > m_queue;
	StretchMode m_stretchMode = StretchMode::Stretch;
};

class DirectDraw7_RwD3D9 final : public IDirectDraw7
{
public:
	// Inherited via IDirectDraw7
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID * ppvObj) override;
	virtual ULONG WINAPI AddRef(void) override;
	virtual ULONG WINAPI Release(void) override;
	virtual HRESULT WINAPI Compact(void) override;					 
	virtual HRESULT WINAPI CreateClipper(DWORD, LPDIRECTDRAWCLIPPER *, IUnknown *) override;					 
	virtual HRESULT WINAPI CreatePalette(DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE *, IUnknown *) override;					 
	virtual HRESULT WINAPI CreateSurface(LPDDSURFACEDESC2 lpDDSurfaceDesc2, LPDIRECTDRAWSURFACE7 * lplpDDSurface, IUnknown * pUnkOuter) override;					 
	virtual HRESULT WINAPI DuplicateSurface(LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7 *) override;					 
	virtual HRESULT WINAPI EnumDisplayModes(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2) override;					 
	virtual HRESULT WINAPI EnumSurfaces(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK7) override;					 
	virtual HRESULT WINAPI FlipToGDISurface(void) override;					 
	virtual HRESULT WINAPI GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps) override;					 
	virtual HRESULT WINAPI GetDisplayMode(LPDDSURFACEDESC2) override;					 
	virtual HRESULT WINAPI GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes) override;					 
	virtual HRESULT WINAPI GetGDISurface(LPDIRECTDRAWSURFACE7 *) override;					 
	virtual HRESULT WINAPI GetMonitorFrequency(LPDWORD) override;			 
	virtual HRESULT WINAPI GetScanLine(LPDWORD) override;				 
	virtual HRESULT WINAPI GetVerticalBlankStatus(LPBOOL) override;				 
	virtual HRESULT WINAPI Initialize(GUID *) override;				 
	virtual HRESULT WINAPI RestoreDisplayMode(void) override;				 
	virtual HRESULT WINAPI SetCooperativeLevel(HWND hWnd, DWORD dwFlags) override;				 
	virtual HRESULT WINAPI SetDisplayMode(DWORD, DWORD, DWORD, DWORD, DWORD) override;		 
	virtual HRESULT WINAPI WaitForVerticalBlank(DWORD, HANDLE) override;				 
	virtual HRESULT WINAPI GetAvailableVidMem(LPDDSCAPS2, LPDWORD, LPDWORD) override;				 
	virtual HRESULT WINAPI GetSurfaceFromDC(HDC, LPDIRECTDRAWSURFACE7 *) override;				 
	virtual HRESULT WINAPI RestoreAllSurfaces(void) override;				 
	virtual HRESULT WINAPI TestCooperativeLevel(void) override;				 
	virtual HRESULT WINAPI GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2, DWORD) override;				 
	virtual HRESULT WINAPI StartModeTest(LPSIZE, DWORD, DWORD) override;
	virtual HRESULT WINAPI EvaluateMode(DWORD, DWORD *) override;

	static DD7_RwD3D9OverlayRenderQueue& OverlayRenderQueue() { return ms_overlayRenderQueue; }

	DirectDraw7_RwD3D9()
	{
		CreateYUY2Shader();
	}

	~DirectDraw7_RwD3D9();

private:
	void CreateYUY2Shader();

	LONG m_refCount = 1;
	void* m_yuy2Shader = nullptr;

	static DD7_RwD3D9OverlayRenderQueue ms_overlayRenderQueue;
};

void InstallRenderQueueHook();