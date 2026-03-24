/*

    Copyright 2024 Sergey Naumenkov

    File: LevelMapDefs.h
    Description: Shared level info for all modules

    Date: 12/08/2024

*/

#ifndef __Shared_Public_LevelMapDefs_h__
#define __Shared_Public_LevelMapDefs_h__
#pragma once

#include <vector>
#include "Math/Vector3D.h"
#include "Math/Vector2D.h"

struct OffsetData_t
{
    int fileOffset;
    int fileLen;
};

struct MapFileCubeSideData_t
{
    Vector3Df vertices[ 24 ];
    Vector2Df uv[ 24 ];
    Vector3Df normal;
    float dist;
};

// Cube data
struct MapFileCubeData_t
{
    bool isColored; // 1 or 0
    MapFileCubeSideData_t cubeSides[ 6 ];
    char mTexturePath[ 512 ];
};

// Map header
struct MapFileHeader_t
{
    int mVersion; // Map header version
    int mMapCompilerVersion; // usually 1
    OffsetData_t offsets[ 1 ]; // right now only for primitives

};

struct CollisionCubeSide_t
{
    Vector3Df normal;
    float dist;
};

struct CollisionCube_t
{
    CollisionCubeSide_t sides[ 6 ];
    bool isDynamic;

    Vector3Df min;
    Vector3Df max;
};

struct MapCollision_t
{
    std::vector<CollisionCube_t> collisionCubes;
};

#endif // !__Shared_Public_LevelMapDefs_h__
