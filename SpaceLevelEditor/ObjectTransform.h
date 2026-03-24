/*

    Copyright 2024 Sergey Naumenkov

    File: ObjectTransform.h
    Description: Transform object in the world space by mouse

    Date: 12/08/2024

*/

#ifndef __SpaceLevelEditor_ObjectTransform_h__
#define __SpaceLevelEditor_ObjectTransform_h__
#pragma once

#include "ObjectTransformDefs.h"

#include "Math/Vector2D.h"
#include "Math/Vector3D.h"

class ObjectTransform
{
public:
	ObjectTransform( );
	~ObjectTransform( );

    // Start transformation
    void Start( void* wnd);

    // Transform object position
    Vector3Df TransformPosition( TransformAxis_e axis, Vector3Df objectPosition, float transformSpeed );

    // Transform object size
    Vector3Df TransformSize( TransformAxis_e axis, Vector3Df objectSize, float transformSpeed );

    // Change current transform mode
    // By default it's move
    void SetTransformMode( TransformMode_e mode );

    // End transformation
    void End( );

private:
    Vector2D mCurrentMousePosition;
    Vector2D mOldMousePosition;

    TransformMode_e mCurrentTransformMode;
};

#endif // !__SpaceLevelEditor_ObjectTransform_h__
