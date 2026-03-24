/*

    Copyright 2024 Sergey Naumenkov

    File: DirectX11States.h
    Description: Create/bind and etc with DirectX 11 states

    Date: 28/08/2024

*/

#ifndef __RenderDx11_DirectX11States_h__
#define __RenderDx11_DirectX11States_h__
#pragma once

#include <d3d11_1.h>

#include "Math\Vector2D.h"

class DirectX11States
{
public:
	DirectX11States( );

    // Initialize 
    void Initialize( ID3D11Device1* device, ID3D11DeviceContext1* context );

    // Create blend state
    // createDisableState - create second instance for disabling the blend state
    bool CreateBlendStates( bool createDisableState );

    // Bind blend state
    // bindNormalBlend = bind enabled or disabled blend state
    void BindBlendState( bool bindNormalBlend );

    // Create Rasterizer state
    bool CreateRasterizerState( );

    // Bind Rasterizer state
    // useWireframe - bind wireframe rasterizer state
    void BindRasterizerState( bool useWireframe );

    // Create Depth test
    // createDisabledState - create second instance for disabling the depth test
    bool CreateDepthTest( bool createDisabledState, int viewportWidth, int viewportHeight );

    // Bind Depth test
    // bindNormalDepthTest = bind enabled or disabled Depth test
    void BindDepthTest( bool bindNormalDepthTest );

    // Get Depth Stencil View
    // It's need for binding in 'Start Frame'
    ID3D11DepthStencilView* GetDepthView( );

    // Create default texture sampler
    bool CreateDefaultTextureSampler( );

    // Get default sampler state
    ID3D11SamplerState* GetDefaultSamplerState( );

    // Update Render scissors
    void UpdateRenderScissors( Vector2D pos, Vector2D size );

    // Release all states
    void Shutdown( );

private:
    // Blend state
    ID3D11BlendState1* mBlendStateOn;
    ID3D11BlendState1* mBlendStateOff;

    // Rasterizer state
    ID3D11RasterizerState1* mRasterizerStateSolid;
    ID3D11RasterizerState1* mRasterizerStateWireframe;

    // Depth test
    ID3D11DepthStencilState* mDepthStencilOn;
    ID3D11DepthStencilState* mDepthStencilOff;
    ID3D11DepthStencilView* mDepthView;
    bool mUseDepthTest;

    // Sampler state
    ID3D11SamplerState* mDefaultSamplerState;

    // Render devices
    ID3D11Device1* mDevice;
    ID3D11DeviceContext1* mDeviceContext;
};

#endif // !__RenderDx11_DirectX11States_h__
