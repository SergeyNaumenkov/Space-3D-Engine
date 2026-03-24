/*

    Copyright 2024 Sergey Naumenkov

    File: RayCast.h
    Description: Ray cast 

    Date: 11/08/2024

*/

#ifndef __SpaceLevelEditor_RayCast_h__
#define __SpaceLevelEditor_RayCast_h__
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

#include "3DCamera.h"

class RayCast
{
public:
	RayCast( );
	~RayCast( );

    // Convert screen mouse position to world position
    XMVECTOR GetWorldMousePosition( int mouseX, int mouseY, int screenWidth, int screenHeight, Render3DCamera* camera );

    // Cast ray in to rayDirection
    // If box is detected - return true
    // Otherwise - false
    // distanceToObject: Distance from ray origin to object
    bool CastRayForBox( XMVECTOR rayOrigin, XMVECTOR rayDirection, Vector3Df bboxMin, Vector3Df bboxMax, float& distanceToObject );

private:

};

#endif // !__SpaceLevelEditor_RayCast_h__
