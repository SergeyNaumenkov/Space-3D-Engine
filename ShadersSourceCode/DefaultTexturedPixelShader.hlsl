/*

	Copyright 2024 Sergey Naumenkov

	File: DefaultTexturedPixelShader.hlsl
	Description: Default vertex shader for textured primitives.

	Date: 10/08/2024

*/

#include "SharedShaderData.hlsli"

Texture2D baseTexture;
SamplerState samLinear;

struct DefaultPixelOutput_t
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

float4 main(DefaultPixelOutput_t input) : SV_Target
{
    float3 normal = normalize(input.normal);
    float4 finalColor = float4(0, 0, 0, 0);
    float3 LightColors = float3(1, 1, 1);
    float3 lightPosition = float3(-25, 0, -40);
    
    float diffuseIntensity = max(dot(normal, lightPosition), 0.0f);
    float3 diffuseColor = LightColors * diffuseIntensity * 0.01f;
    
    // Normalize ambient color
    float4 normalizedAmbientColor = mAmbientColor / 255.f;
    finalColor += baseTexture.Sample(samLinear, input.uv) * normalizedAmbientColor + float4(diffuseColor, 1);
    
    if (mIsAmbientLightEnable == 1)
        return finalColor;
    else
        return baseTexture.Sample(samLinear, input.uv);

}