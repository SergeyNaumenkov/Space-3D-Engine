/*

    Copyright 2024 Sergey Naumenkov

    File: Vector4D.h
    Description: Vector4D implementation

    Date: 04/08/2024

*/

#ifndef __Math_Vector4D_h__
#define __Math_Vector4D_h__
#pragma once

// Vector4D based on INT variables
struct Vector4D
{
    Vector4D( )
    {
        x = y = z = w = 0;
    }

    Vector4D( int _x, int _y, int _z, int _w )
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    int x;
    int y;
    int z;
    int w;
};

// Vector4D based on FLOAT variables
struct Vector4Df
{
    Vector4Df( )
    {
        x = y = z = w = 0;
    }

    Vector4Df( float _x, float _y, float _z, float _w )
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    float x;
    float y;
    float z;
    float w;
};

#endif // !__Math_Vector4D_h__
