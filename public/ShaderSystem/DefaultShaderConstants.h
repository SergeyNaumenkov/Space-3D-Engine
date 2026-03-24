/*

    Copyright 2024 Sergey Naumenkov

    File: DefaultShaderConstants
    Description: Default shader constants

    Date: 17/08/2024

*/

#ifndef __ShaderSystemDx11_DefaultShaderConstants_h__
#define __ShaderSystemDx11_DefaultShaderConstants_h__
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

struct DefaultShaderConstantBuffer_t
{
    XMMATRIX view;
    XMMATRIX projection;
    XMMATRIX world;
};

#endif // !__ShaderSystemDx11_DefaultShaderConstants_h__
