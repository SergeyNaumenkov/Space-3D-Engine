/*

    Copyright 2024 Sergey Naumenkov

    File: main.cpp
    Description: Math library

    Date: 04/08/2024

*/

#include "Math.h"

/*
    Get min or max value 
*/
float Clamp( float value, float min, float max )
{
    if( value > max )
        return max;

    if( value < min )
        return min;

    return value;
}
