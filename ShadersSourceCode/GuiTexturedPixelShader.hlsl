/*

	Copyright 2024 Sergey Naumenkov

	File: GuiTexturedPixelShader.hlsl
	Description: Shader for rendering 2D textured quads 

	Date: 18/08/2024

*/

Texture2D baseTexture;
SamplerState samLinear;

struct GuiTexturedPixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

float4 main(GuiTexturedPixelInput input) : SV_Target
{
    float4 color = baseTexture.Sample(samLinear, input.uv) * input.color;
    return color;
}