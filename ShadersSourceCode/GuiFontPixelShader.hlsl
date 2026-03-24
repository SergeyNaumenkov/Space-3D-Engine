/*

	Copyright 2024 Sergey Naumenkov

	File: GuiFontPixelShader.hlsl
	Description: Shader for rendering 2D text 

	Date: 21/08/2024

*/

Texture2D baseTexture;
SamplerState samLinear;

struct Default2DPixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

float4 main(Default2DPixelInput input) : SV_Target
{
    float4 color =  baseTexture.Sample(samLinear, input.uv) * input.color;
    return color;
}