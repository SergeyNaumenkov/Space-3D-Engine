/*

	Copyright 2024 Sergey Naumenkov

	File: GuiFontVertexShader.hlsl
	Description: Shader for rendering 2D text 

	Date: 21/08/2024

*/

cbuffer GlobalConstantBuffer
{
    matrix orthoMatrix;
}

struct Default2DVertexInput
{
    float2 position : POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

struct Default2DPixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

Default2DPixelInput main(Default2DVertexInput input)
{
    Default2DPixelInput output;
    output.position = mul(float4(input.position, 0, 1), orthoMatrix);
    output.uv = input.uv;
    output.color = input.color;
    
    return output;
}