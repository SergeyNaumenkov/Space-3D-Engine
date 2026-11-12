/*

    Copyright 2024 Sergey Naumenkov

    File: Mesh.h
    Description: Render mesh

    Date: 25/08/2024

*/

#ifndef __RenderDx11_Mesh_h__
#define __RenderDx11_Mesh_h__
#pragma once

#include <d3d11_1.h>

#include "Mesh\IMesh.h"
#include "BaseRenderer\VertexBufferBuilder.h"
#include "BaseRenderer\IndexBufferBuilder.h"

class Mesh : public IMesh
{
public:
    Mesh( );
	~Mesh( );

    // Lock mesh
    virtual void LockMesh( MeshType_t meshType, MeshRenderType_t renderType )  override;

    // Add vertex position
    virtual void AddPositionFloat3( Vector3Df position ) override;

    // Add vertex uv
    virtual void AddUVFloat2( Vector2Df uv ) override;

    // Add vertex color
    virtual void AddColor( unsigned long color ) override;

    // Add vertex normal
    virtual void AddNormalFloat3( Vector3Df normal ) override;

    // Unlock Mesh
    virtual void UnlockMesh( )  override;

    // Draw mesh
    virtual void DrawMesh( ) override;

    // Create render buffer
    void MakeRenderBuffer( );

protected:

    // Update vertex buffer
    void UpdateVertexBuffer( );

    // Draw batch from vertex buffer
    void DrawBatch( int verticesCount );

private:
    VertexBufferBuilder mVertexBuffer;
    IndexBufferBuilder mIndexBuffer;

    ID3D11Buffer* mVertexRenderBuffer;
    ID3D11Buffer* mIndexRenderBuffer;

    // Render information
    MeshType_t mMeshType;
    MeshRenderType_t mMeshRenderType;
};

#endif // !__RenderDx11_Mesh_h__
