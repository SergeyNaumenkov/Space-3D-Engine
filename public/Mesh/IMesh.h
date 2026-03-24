/*

    Copyright 2024 Sergey Naumenkov

    File: IMesh.h
    Description: Public interface for mesh rendering

    Date: 25/08/2024

*/

#ifndef __Mesh_IMesh_h__
#define __Mesh_IMesh_h__
#pragma once

#include "Math/Vector2D.h"
#include "Math/Vector3D.h"

enum class MeshType_t
{
    Mesh_3D,
    Mesh_2D,
    Mesh_2DFont // Special for render 2D font
};

enum class MeshRenderType_t
{
    Render_Triangles,
    Render_Lines,
    Render_Points
};

class IMesh
{
public:

    // Lock mesh
    virtual void LockMesh(MeshType_t meshType, MeshRenderType_t renderType ) = 0;

    // Add vertex position
    virtual void AddPositionFloat3( Vector3Df position ) = 0;

    // Add vertex normal
    virtual void AddNormalFloat3( Vector3Df normal ) = 0;

    // Add vertex uv
    virtual void AddUVFloat2( Vector2Df uv ) = 0;

    // Add vertex color
    virtual void AddColor( unsigned long color ) = 0;

    // Unlock Mesh
    virtual void UnlockMesh( ) = 0;

    // Draw mesh
    virtual void DrawMesh( ) = 0;

};

#endif // !__Mesh_IMesh_h__
