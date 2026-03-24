/*

	Copyright 2024 Sergey Naumenkov

	File: GuiTexturedVertexShader.hlsl
	Description: Shader for rendering 2D textured quads 

	Date: 18/08/2024

*/

cbuffer GlobalConstantBuffer
{
    matrix orthoMatrix;
};

struct GuiTexturedVertexInput
{
    float2 position : POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

struct GuiTexturedPixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

GuiTexturedPixelInput main(GuiTexturedVertexInput input)
{
    GuiTexturedPixelInput output;
    output.position = mul(float4(input.position, 0, 1), orthoMatrix);
    output.color = input.color;
    output.uv = input.uv;
    
    return output;
}