/*

	Copyright 2024 Sergey Naumenkov

	File: SharedShaderData.hlsl
	Description: Shared defs for shaders

	Date: 20/08/2024

*/

cbuffer AmbientLight
{
	// Color
    float4 mAmbientColor;
	
	// Enable or disable ambient light
    int mIsAmbientLightEnable;
};