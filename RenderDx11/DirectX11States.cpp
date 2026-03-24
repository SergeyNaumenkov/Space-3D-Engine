/*

	Copyright 2024 Sergey Naumenkov

	File: DirectX11States.cpp
	Description: Create/bind and etc with DirectX 11 states

	Date: 28/08/2024

*/

#include "DirectX11States.h"

#include "Core/Debug.h"

/*
	Constructor
*/
DirectX11States::DirectX11States( )
{
	mBlendStateOn = mBlendStateOff = nullptr;
	mRasterizerStateSolid = mRasterizerStateWireframe = nullptr;
	mDepthStencilOn = mDepthStencilOff = nullptr;
	mDepthView = nullptr;

	mDevice = nullptr;
	mDeviceContext = nullptr;
}

/*
	Initialize
*/
void DirectX11States::Initialize( ID3D11Device1* device, ID3D11DeviceContext1* context )
{
	mDevice = device;
	mDeviceContext = context;
}

/*
	Create blend state

	params:
		createDisableState - create second instance for disabling the blend state
*/
bool DirectX11States::CreateBlendStates( bool createDisableState )
{
	D3D11_BLEND_DESC1 blendDesc = { };
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[ 0 ].BlendEnable = true;
	blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	// Create normal blend state
	auto result = mDevice->CreateBlendState1( &blendDesc, &mBlendStateOn );
	if( FAILED( result ) )
	{
		PrintError( "Failed create blend state!" );
		return false;
	}

	// Create disabled blend state
	if( createDisableState )
	{
		// Disable it
		blendDesc.RenderTarget[ 0 ].BlendEnable = false;
		result = mDevice->CreateBlendState1( &blendDesc, &mBlendStateOff );
		if( FAILED( result ) )
		{
			PrintError( "Failed create blend state!" );
			return false;
		}
	}

	return true;
}

/*
	Bind blend state

	params:
		bindNormalBlend = bind enabled or disabled blend state
*/
void DirectX11States::BindBlendState( bool bindNormalBlend )
{
	if( bindNormalBlend )
	{
		mDeviceContext->OMSetBlendState( mBlendStateOn, 0, 0xFFFFFFFF );
	}
	else
	{
		mDeviceContext->OMSetBlendState( mBlendStateOff, 0, 0xFFFFFFFF );
	}
}

/*
	Create Rasterizer state
*/
bool DirectX11States::CreateRasterizerState( )
{
	D3D11_RASTERIZER_DESC1 rasterizerDesc = { };
	rasterizerDesc.AntialiasedLineEnable = true;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.DepthClipEnable = false;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = true;
	rasterizerDesc.ScissorEnable = true;
	rasterizerDesc.SlopeScaledDepthBias = 0;

	// Create it
	auto result = mDevice->CreateRasterizerState1( &rasterizerDesc, &mRasterizerStateSolid );
	if( FAILED( result ) )
	{
		PrintError( "Failed create rasterizer state!" );
		return false;
	}

	// Create state for wireframe
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	result = mDevice->CreateRasterizerState1( &rasterizerDesc, &mRasterizerStateWireframe );
	if( FAILED( result ) )
	{
		PrintError( "Failed create rasterizer state!" );
		return false;
	}

	return true;
}

/*
	Bind Rasterizer state

	params:
		useWireframe - bind wireframe rasterizer state
*/
void DirectX11States::BindRasterizerState( bool useWireframe )
{
	if( useWireframe )
	{
		mDeviceContext->RSSetState( mRasterizerStateWireframe );
	}
	else
	{
		mDeviceContext->RSSetState( mRasterizerStateSolid );
	}
}

/*
	Create Depth test

	params:
		createDisabledState - create second instance for disabling the depth test
*/
bool DirectX11States::CreateDepthTest( bool createDisabledState, int viewportWidth, int viewportHeight )
{
	// Create depth texture
	ID3D11Texture2D* depthTexture = NULL;
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = viewportWidth;
	depthDesc.Height = viewportHeight;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24 bits for depth
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	auto result = mDevice->CreateTexture2D( &depthDesc, nullptr, &depthTexture );
	if( FAILED( result ) )
	{
		PrintError( "Failed create depth test texture 2D!" );
		return false;
	}

	// Create depth stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = mDevice->CreateDepthStencilState( &dsDesc, &mDepthStencilOn );
	if( FAILED( result ) )
	{
		PrintError( "Failed create Depth Stencil!" );
		return false;
	}

	// Create disabled state
	if( createDisabledState )
	{
		dsDesc.StencilEnable = false;
		result = mDevice->CreateDepthStencilState( &dsDesc, &mDepthStencilOff );
		if( FAILED( result ) )
		{
			PrintError( "Failed create Depth Stencil!" );
			return false;
		}
	}

	// Create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthView = {};
	depthView.Format = depthDesc.Format;
	depthView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthView.Texture2D.MipSlice = 0;

	result = mDevice->CreateDepthStencilView( depthTexture, &depthView, &mDepthView );
	if( FAILED( result ) )
	{
		PrintError( "Failed create depth Stencil View!" );
		return false;
	}

	return true;
}

/*
	Bind Depth test

	params:
		bindNormalDepthTest = bind enabled or disabled Depth test
*/
void DirectX11States::BindDepthTest( bool bindNormalDepthTest )
{
	if( bindNormalDepthTest )
		mDeviceContext->OMSetDepthStencilState( mDepthStencilOn, 1 );
	else
		mDeviceContext->OMSetDepthStencilState( mDepthStencilOff, 1 );

	mUseDepthTest = bindNormalDepthTest;
}

/*
	Get Depth Stencil View
	It's need for binding in 'Start Frame'
*/
ID3D11DepthStencilView* DirectX11States::GetDepthView( )
{
	if( mUseDepthTest )
		return mDepthView;
	else
		return nullptr;
}

/*
	Create default texture sampler
*/
bool DirectX11States::CreateDefaultTextureSampler( )
{
	D3D11_SAMPLER_DESC defSamplerState = { };
	defSamplerState.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	defSamplerState.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	defSamplerState.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	defSamplerState.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	defSamplerState.MinLOD = 0;
	defSamplerState.MaxLOD = FLT_MAX;

	// Create
	auto result = mDevice->CreateSamplerState( &defSamplerState, &mDefaultSamplerState );
	if( FAILED( result ) )
	{
		PrintError( "Failed create default sampler state!" );
		return false;
	}

	return true;
}

/*
	Get default sampler state
*/
ID3D11SamplerState* DirectX11States::GetDefaultSamplerState( )
{
	return mDefaultSamplerState;
}

/*
	Update Render scissors
*/
void DirectX11States::UpdateRenderScissors( Vector2D pos, Vector2D size )
{
	D3D11_RECT rect;
	rect.left = static_cast< LONG >( pos.x );
	rect.right = static_cast< LONG >( pos.x + size.x );

	rect.top = static_cast< LONG >( pos.y );
	rect.bottom = static_cast< LONG >( pos.y + size.y );

	mDeviceContext->RSSetScissorRects( 1, &rect );
}

/*
	Release all states
*/
void DirectX11States::Shutdown( )
{
#define DXSafeRelease(_ptr) if(_ptr){_ptr->Release(); _ptr=nullptr;}

	DXSafeRelease( mBlendStateOn );
	DXSafeRelease( mBlendStateOff );
	DXSafeRelease( mRasterizerStateSolid );
	DXSafeRelease( mRasterizerStateWireframe );
	DXSafeRelease( mDepthStencilOff );
	DXSafeRelease( mDefaultSamplerState );
}
