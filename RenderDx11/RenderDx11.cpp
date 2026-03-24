/*

	Copyright 2024 Sergey Naumenkov

	File: RenderDx11.cpp
	Description: Render based on DirectX 11

	Date: 24/08/2024

*/

#include <d3d11_1.h>
#include "BaseRenderer\IBaseRenderer.h"

#include "InterfaceFactory\InterfaceFactory.h"
#include "Core\Debug.h"
#include "Core\TemplateConverter.h"

#include "Mesh.h"
#include "Mesh2D.h"
#include "RenderDevicesInternal.h"
#include "DirectX11States.h"
#include "Display.h"

#include "MaterialLoader.h"

RenderDevicesInternal_t* g_RenderDevicesInternal = nullptr;

// DEFINES IS EVIL!
#define SafeRelease(_ptr) {if(_ptr) {_ptr->Release(); _ptr = nullptr;}}

class RenderDx11 : public IBaseRenderer
{
public:
	RenderDx11( );

	// Get Render Tag
	virtual RendererTag_t GetTag( );

	// Get render device ( if Driver is supported )
	virtual void* GetDevice( );

	// Initialize render system
	virtual bool Initialize( BaseRenderData_t& baseRenderData ) override;

	// Start frame 
	virtual void StartFrame( ) override;

	// End Frame
	virtual void EndFrame( ) override;

	// Shutdown render system
	virtual void Shutdown( ) override;

	// Get render mesh
	virtual IMesh* GetRenderMesh( ) override;

	// Get 2D render mesh
	virtual IMesh2D* GetRenderMesh2D( ) override;

	// Enable or disable Depth test
	virtual void SetDepthState( bool state ) override;

	// Enable or disable Blend state
	virtual void SetBlendState( bool state ) override;

	// Set Render scissors
	virtual void SetScissors( bool state, Vector2D pos, Vector2D size ) override;

	// Load material 
	virtual MaterialData_t* LoadMaterial( const char* materialPath, const char* materialName ) override;

	// Release material
	virtual void ReleaseMaterial( MaterialData_t* data ) override;

protected:

	// Create basic devices
	bool CreateDevices( );

	// Get devices stage 2
	bool FactorySetting( ID3D11Device* device, IDXGIFactory2*& factory2 );

	// Create swapchain
	bool CreateSwapChain( );

	// Update viewport
	void LinkViewport( float w = -1, float h = -1 );

	// Create backbuffer for rendering
	bool CreateBackBuffer( );

private:
	ID3D11Device1* mDevice;
	ID3D11DeviceContext1* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;

	IDXGISwapChain1* mSwapChain;
	IDXGIFactory2* mDxgiFactory2;

	BaseRenderData_t* mRenderData;
	Display* mDxgiDisplay;

	Mesh* mRenderMesh;
	Mesh2D* mRenderMesh2D;

	DirectX11States* mRenderStates;
	MaterialLoader* mMaterialLoader;
};

/*
	Constructor
*/
RenderDx11::RenderDx11( )
{
	mDevice = nullptr;
	mDeviceContext = nullptr;
	mRenderTargetView = nullptr;
	mSwapChain = nullptr;
	mDxgiFactory2 = nullptr;

	mRenderData = nullptr;
	mDxgiDisplay = nullptr;

	mRenderMesh = nullptr;
	mRenderMesh2D = nullptr;

	mRenderStates = nullptr;
	mMaterialLoader = nullptr;
}

/*
	Get Render Tag
*/
RendererTag_t RenderDx11::GetTag( )
{
	return RendererTag_t::RenderTag_DirectX11;
}

/*
	Get render device ( if Driver is supported )
*/
void* RenderDx11::GetDevice( )
{
	return mDevice;
}

/*
	Initialize render system
*/
bool RenderDx11::Initialize( BaseRenderData_t& baseRenderData )
{
	mRenderData = new BaseRenderData_t( baseRenderData );

	if( !CreateDevices( ) )
		return false;

	if( !CreateSwapChain( ) )
		return false;

	if( !CreateBackBuffer( ) )
		return false;

	LinkViewport( );

	// Get display modes
	mDxgiDisplay = new Display( );
	if( !mDxgiDisplay->Initialize( mDxgiFactory2 ) )
		return false;

	// Copy render devices for internal uses
	g_RenderDevicesInternal = new RenderDevicesInternal_t( );
	g_RenderDevicesInternal->mDevice = mDevice;
	g_RenderDevicesInternal->mDeviceContext = mDeviceContext;

	// Create render states
	mRenderStates = new DirectX11States( );
	mRenderStates->Initialize( mDevice, mDeviceContext );

	// Create blend state
	mRenderStates->CreateBlendStates( true );
	mRenderStates->BindBlendState( true );

	// Create depth state
	mRenderStates->CreateDepthTest( true, mRenderData->mWindowWidth, mRenderData->mWindowHeight );
	mRenderStates->BindDepthTest( true );

	// Create rasterizer state
	mRenderStates->CreateRasterizerState( );
	mRenderStates->BindRasterizerState( false );

	// Create sampler state
	mRenderStates->CreateDefaultTextureSampler( );

	// Set render scissors bounds
	Vector2D scSize = { mRenderData->mWindowWidth,mRenderData->mWindowHeight };
	Vector2D scPos = { 0,0 };
	mRenderStates->UpdateRenderScissors( scPos, scSize );

	// Create material loader
	mMaterialLoader = new MaterialLoader( );

	// Load File System
	g_FileSystem = GetClassFromDll<IFileSystem*>( "FileSystem.dll", "IFileSystemV1" );
	if( !g_FileSystem )
	{
		return false;
	}

	return true;
}

/*
	Start frame
*/
void RenderDx11::StartFrame( )
{
	// Clear old pixels
	float clr[ 4 ] = { 0.f / 255.f, 0.f / 255.f, 0.f / 255.f, 1.f };
	mDeviceContext->ClearRenderTargetView( mRenderTargetView, clr );

	// Bind render target view and depth test(if enabled)
	auto depthView = mRenderStates->GetDepthView( );
	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, depthView );

	// If depth stencil is enable when clear frame for depth test
	if( depthView )
		mDeviceContext->ClearDepthStencilView( depthView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

/*
	End Frame
*/
void RenderDx11::EndFrame( )
{
	DXGI_PRESENT_PARAMETERS params = { };
	auto result = mSwapChain->Present1( 0, DXGI_PRESENT_ALLOW_TEARING, &params );
	if( FAILED( result ) )
	{
		auto reason = mDevice->GetDeviceRemovedReason( );
		PrintMessage( "Device is removed!" );
	}
}

/*
	Shutdown render system
*/
void RenderDx11::Shutdown( )
{
	SafeRelease( mDevice );

	mDeviceContext->ClearState( );
	SafeRelease( mDeviceContext );
	SafeRelease( mRenderTargetView );
	SafeRelease( mSwapChain );
	SafeRelease( mDxgiFactory2 );

	delete mRenderData;
	mRenderData = nullptr;

	if( g_RenderDevicesInternal )
	{
		delete g_RenderDevicesInternal;
		g_RenderDevicesInternal = nullptr;
	}

	if( mRenderMesh )
	{
		delete mRenderMesh;
		mRenderMesh = nullptr;
	}

	if( mRenderMesh2D )
	{
		delete mRenderMesh2D;
		mRenderMesh2D = nullptr;
	}

	// Shutdown states
	mRenderStates->Shutdown( );
	delete mRenderStates;
}

/*
	Get render mesh
*/
IMesh* RenderDx11::GetRenderMesh( )
{
	if( !mRenderMesh )
	{
		mRenderMesh = new Mesh( );
		mRenderMesh->MakeRenderBuffer( );
	}

	return mRenderMesh;
}

/*
	Get 2D render mesh
*/
IMesh2D* RenderDx11::GetRenderMesh2D( )
{
	if( !mRenderMesh2D )
	{
		mRenderMesh2D = new Mesh2D( );
		mRenderMesh2D->MakeRenderBuffers( );
	}

	return mRenderMesh2D;
}

/*
	Enable or disable Depth test
*/
void RenderDx11::SetDepthState( bool state )
{
	mRenderStates->BindDepthTest( state );
}

/*
	Enable or disable Blend state
*/
void RenderDx11::SetBlendState( bool state )
{
	mRenderStates->BindBlendState( state );
}

/*
	Set Render scissors
*/
void RenderDx11::SetScissors( bool state, Vector2D pos, Vector2D size )
{
	mRenderStates->UpdateRenderScissors( pos, size );
}

/*
	Load material 
*/
MaterialData_t* RenderDx11::LoadMaterial( const char* materialPath, const char* materialName )
{
	return mMaterialLoader->LoadMaterial( materialPath, materialName );
}

/*
	Release material
*/
void RenderDx11::ReleaseMaterial( MaterialData_t* data )
{
	if( !data )
		return;

	if( data->mTexture )
	{
		ConvertToNewValue<ID3D11ShaderResourceView*>( data->mTexture )->Release( );
		data->mTexture = nullptr;
	}
}

/*
	Create basic devices
*/
bool RenderDx11::CreateDevices( )
{
	// Feature flags
	D3D_FEATURE_LEVEL featureLevels[ ] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D_FEATURE_LEVEL currentLevel;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	// Create DirectX 11 devices
	HRESULT result = D3D11CreateDevice( nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, featureLevels, ARRAYSIZE( featureLevels ), D3D11_SDK_VERSION, &device, &currentLevel, &deviceContext );
	if( FAILED( result ) )
	{
		PrintError( "Failed create DirectX 11 device" );
		return false;
	}

	if( !FactorySetting( device, mDxgiFactory2 ) )
		return false;

	return true;
}

/*
	Get devices stage 2
*/
bool RenderDx11::FactorySetting( ID3D11Device* device, IDXGIFactory2*& factory2 )
{
	IDXGIFactory1* pDxgiFactory1 = nullptr;
	IDXGIDevice1* pDxgiDevice = nullptr;
	auto result = device->QueryInterface( __uuidof( IDXGIDevice1 ), reinterpret_cast< void** >( &pDxgiDevice ) );
	if( FAILED( result ) )
		return false;

	// Get adapter information
	IDXGIAdapter* pDxgiAdapter = nullptr;
	pDxgiDevice->GetAdapter( &pDxgiAdapter );

	// Get Factory
	result = pDxgiAdapter->GetParent( __uuidof( IDXGIFactory1 ), reinterpret_cast< void** >( &pDxgiFactory1 ) );
	if( FAILED( result ) )
		return false;

	pDxgiDevice->SetMaximumFrameLatency( 1 );

#if _Debug
	DXGI_ADAPTER_DESC desc;
	pDxgiAdapter->GetDesc( &desc );
	PrintNote( "Adapter description: %ls", desc.Description );
#endif
	pDxgiAdapter->Release( );
	pDxgiDevice->Release( );

	// Get factory2
	result = pDxgiFactory1->QueryInterface( __uuidof( IDXGIFactory2 ), reinterpret_cast< void** >( &factory2 ) );
	if( FAILED( result ) )
		return false;

	result = device->QueryInterface( __uuidof( ID3D11Device1 ), reinterpret_cast< void** >( &mDevice ) );
	if( FAILED( result ) )
		return false;

	mDevice->GetImmediateContext1( &mDeviceContext );

	pDxgiFactory1->Release( );

	return true;
}

/*
	Create swapchain
*/
bool RenderDx11::CreateSwapChain( )
{
	// Create SwapChain
	DXGI_SWAP_CHAIN_DESC1 scd = { };
	scd.Width = static_cast< UINT >( mRenderData->mWindowWidth );
	scd.Height = static_cast< UINT >( mRenderData->mWindowHeight );

	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.Stereo = FALSE;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	

	scd.Scaling = DXGI_SCALING_NONE;

	// Microsoft DX11 team recommend use flip model, because
	// this model give less latency
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	auto result = mDxgiFactory2->CreateSwapChainForHwnd( mDevice, ( HWND )mRenderData->mWindowPtr, &scd, nullptr, nullptr, &mSwapChain );

	if( FAILED( result ) )
	{
		PrintError( "Failed create swapchain!" );
		return false;
	}

	DXGI_MODE_DESC desiredMode = {};
	desiredMode.Width = scd.Width;
	desiredMode.Height = scd.Height;
	desiredMode.Format = scd.Format;
	desiredMode.RefreshRate.Numerator = 240;
	desiredMode.RefreshRate.Denominator = 1;

	HRESULT hr = mSwapChain->ResizeTarget( &desiredMode );

	return true;
}

/*
	Update viewport
*/
void RenderDx11::LinkViewport( float w, float h )
{
	// used default values
	if( w == -1 || h == -1 )
	{
		w = static_cast< float >( mRenderData->mWindowWidth );
		h = static_cast< float >( mRenderData->mWindowHeight );
	}

	D3D11_VIEWPORT vp = { };
	vp.Width = w;
	vp.Height = h;
	vp.MaxDepth = 1.0f;

	// Attach new viewport
	mDeviceContext->RSSetViewports( 1, &vp );
}

/*
	Create backbuffer for rendering
*/
bool RenderDx11::CreateBackBuffer( )
{
	ID3D11Texture2D* pBackBuffer = nullptr;
	auto result = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( void** )&pBackBuffer );
	if( FAILED( result ) )
	{
		PrintError( "Failed get back buffer!" );
		return false;
	}


	// Create it
	result = mDevice->CreateRenderTargetView( pBackBuffer, 0, &mRenderTargetView );
	if( FAILED( result ) )
	{
		return false;
	}

	pBackBuffer->Release( );

	return true;
}

RenderDevicesInternal_t*& GetRenderDevicesInternal( )
{
	return g_RenderDevicesInternal;
}

SaveDllClass( RenderDx11, IBaseRenderDirectX11 )