/*

    Copyright 2024 Sergey Naumenkov

    File: IMesh2D.h
    Description: Public interface for 2D meshes

    Date: 27/08/2024

*/

#ifndef __Mesh_IMesh2D_h__
#define __Mesh_IMesh2D_h__
#pragma once

#include "math/Vector2D.h"

enum class Mesh2DType_t
{
    Mesh_Solid,
    Mesh_Font
};

class IMesh2D
{
public:

    // Lock/Unlock mesh
    virtual void LockMesh( Mesh2DType_t type ) = 0;
    virtual void UnlockMesh( ) = 0;

    // Add vertex position
    virtual void AddPositionFloat2( Vector2Df position ) = 0;

    // Add vertex uv
    virtual void AddVertexUVFloat2( Vector2Df uv ) = 0;

    // Add vertex color
    virtual void AddVertexColor( unsigned long color ) = 0;

    // Set color mesh
    virtual void SetMeshAsColor( bool state ) = 0;

    // Draw mesh
    virtual void DrawMesh( ) = 0;

};

#endif // !__Mesh_IMesh2D_h__
