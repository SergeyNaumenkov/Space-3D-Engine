/*

    Copyright 2024 Sergey Naumenkov

    File: ColoredConstantBuffer.h
    Description: Constant color buffer for 2D

    Date: 10/09/2024

*/

#ifndef __public_ColoredConstantBuffer_h__
#define __public_ColoredConstantBuffer_h__
#pragma once

#include <DirectXMath.h>

struct ColorGuiConstantBuffer_t
{
    DirectX::XMMATRIX guiOrthoMatrix;
};

#endif // !__public_ColoredConstantBuffer_h__
