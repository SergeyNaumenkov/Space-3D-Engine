/*

    Copyright 2024 Sergey Naumenkov

    File: Vector2D.h
    Description: Vector2D implementation

    Date: 04/08/2024

*/

#ifndef __Math_Vector2D_h__
#define __Math_Vector2D_h__
#pragma once

// Vector2D based on INT variables
struct Vector2D
{
    Vector2D( )
    {
        x = y = 0;
    }

    Vector2D( int _x, int _y )
    {
        x = _x;
        y = _y;
    }

    int x;
    int y;
};

// Vector2D based on FLOAT variables
struct Vector2Df
{
    Vector2Df( )
    {
        x = y = 0.0f;
    }

    Vector2Df( float _x, float _y )
    {
        x = _x;
        y = _y;
    }

    bool operator==( Vector2Df v )
    {
        if( x == v.x && y == v.y )
            return true;

        return false;
    }

    float x;
    float y;
};

#endif // !__Math_Vector2D_h__
