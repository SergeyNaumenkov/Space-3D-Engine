/*

	Copyright 2024 Sergey Naumenkov

	File: DefaultTexturedVertexShader.hlsl
	Description: Default vertex shader for textured primitives.

	Date: 10/08/2024

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
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct DefaultPixelOutput_t
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

DefaultPixelOutput_t main(DefaultVertexOutput_t input)
{
    DefaultPixelOutput_t pixelOutput = (DefaultPixelOutput_t) 0;
    
    // Apply matrix transformation
    float4 worldPosition = mul(mWorld, input.position);
    float4 viewPosition = mul(mView, worldPosition);
    pixelOutput.position = mul(mProjection, viewPosition);
    
    // Copy texture uv
    pixelOutput.uv = input.uv;
    pixelOutput.normal = input.normal;

    return pixelOutput;
}