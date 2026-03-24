/*

    Copyright 2024 Sergey Naumenkov

    File: ShaderBundle.h
    Description: Shaders Bundle

    Date: 03/08/2024

*/

#ifndef __ShaderSystem_ShaderBundle_h__
#define __ShaderSystem_ShaderBundle_h__
#pragma once

#include <d3d11_1.h>

struct ShaderBundle_t
{
    const char* mBundleName;

    ID3D11VertexShader* mVertexShader;
    ID3D11InputLayout* mVertexLayout;
    ID3D11PixelShader* mPixelShader;
    ID3D11GeometryShader* mGeometryShader;
};

#endif // !__ShaderSystem_ShaderBundle_h__
