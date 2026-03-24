/*

	Copyright 2024 Sergey Naumenkov

	File: Default2DPixelShader.hlsl
	Description: Default pixel shader for colored 2D objects

	Date: 18/08/2024

*/

struct Default2DPixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(Default2DPixelInput input) : SV_Target
{
    return input.color;
}