/*

	Copyright 2024 Sergey Naumenkov

	File: Default2DVertexShader.hlsl
	Description: Default vertex shader for colored 2D objects

	Date: 18/08/2024

*/

cbuffer GlobalConstantBuffer
{
    matrix orthoMatrix;
};

struct Default2DVertexInput
{
    float2 position : POSITION;
    float4 color : COLOR;
};

struct Default2DPixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

Default2DPixelInput main(Default2DVertexInput input)
{
    Default2DPixelInput output;
    output.position = mul(float4(input.position, 0, 1), orthoMatrix);
    output.color = input.color;
    
    return output;
}