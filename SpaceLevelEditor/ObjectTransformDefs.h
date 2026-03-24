/*

    Copyright 2024 Sergey Naumenkov

    File: ObjectTransform.h
    Description: Transform object defines

    Date: 12/08/2024

*/

#ifndef __SpaceLevelEditor_ObjectTransformDefs_h__
#define __SpaceLevelEditor_ObjectTransformDefs_h__
#pragma once

// Axis in 3D space
enum class TransformAxis_e
{
    Axis_X,
    Axis_Y,
    Axis_Z
};

// Modes for transform
enum class TransformMode_e
{
    Mode_Move,
    Mode_Size,
    Mode_VertexManipulations // later!
};

#endif // !__SpaceLevelEditor_ObjectTransformDefs_h__
