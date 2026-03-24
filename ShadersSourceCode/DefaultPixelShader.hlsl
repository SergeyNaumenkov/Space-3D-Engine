/*

	Copyright 2024 Sergey Naumenkov

	File: DefaultPixelShader.hlsl
	Description: Default pixel shader for colored primitives.

	Date: 04/08/2024

*/

struct DefaultPixelOutput_t
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

float4 main(DefaultPixelOutput_t input) : SV_Target
{
    return input.color;
}