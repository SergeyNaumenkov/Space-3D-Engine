/*

    Copyright 2024 Sergey Naumenkov

    File: ColorVertex.h
    Description: Colored Vertex for 3D Render

    Date: 04/08/2024

*/

#ifndef __Math_ColorVertex_h__
#define __Math_ColorVertex_h__
#pragma once

#include "Vector3D.h"
#include "Vector2D.h"

struct TexturedVertex_t
{
    Vector3Df position;
    Vector3Df normal;
    Vector2Df uv;
};

struct GuiSolidVertex_t
{
    Vector2Df position;
    Vector2Df uv;
    unsigned long color;
};

struct GuiSolidFontVertex_t
{
    Vector2Df position;
    Vector2Df uv;
    unsigned long color;
};

#endif // !__Math_ColorVertex_h__
