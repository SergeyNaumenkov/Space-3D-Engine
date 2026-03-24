/*

    Copyright 2024 Sergey Naumenkov

    File: BoundingBox.h
    Description: Bounding box

    Date: 16/08/2024

*/

#ifndef __SpaceLevelEditor_BoundingBox_h__
#define __SpaceLevelEditor_BoundingBox_h__
#pragma once

#include "Math/Vector3D.h"

struct BoundingBox_t
{
    Vector3Df min;
    Vector3Df max;
};

#endif // !__SpaceLevelEditor_BoundingBox_h__
