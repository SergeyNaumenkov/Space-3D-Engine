/*

	Copyright 2024 Sergey Naumenkov

	File: DefaultVertexShader.hlsl
	Description: Default vertex shader for colored primitives.

	Date: 04/08/2024

*/

cbuffer VertexConstantBuffer
{
    matrix mWorld;
    matrix mView;
    matrix mProjection;
};

struct DefaultVertexOutput_t
{
    float4 position : Position;
    float4 color : COLOR;
};

struct DefaultPixelOutput_t
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

// entry point
DefaultPixelOutput_t main(DefaultVertexOutput_t input)
{
    DefaultPixelOutput_t pixelOutput = (DefaultPixelOutput_t)0;
    
    // Apply matrix transformation
    float4 worldPosition = mul(input.position, mWorld);
    float4 viewPosition = mul(worldPosition, mView);
    pixelOutput.position = mul(viewPosition, mProjection);
    
    // Copy color
    pixelOutput.color = input.color;

    return pixelOutput;
}