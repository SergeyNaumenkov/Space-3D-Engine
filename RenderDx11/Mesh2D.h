/*

	Copyright 2024 Sergey Naumenkov

	File: Mesh2D.h
	Description: Render mesh 2D

	Date: 27/08/2024

*/

#ifndef __RenderDx11_Mesh2D_h__
#define __RenderDx11_Mesh2D_h__
#pragma once

#include <d3d11_1.h>

#include "Mesh\IMesh2D.h"
#include "BaseRenderer\IndexBufferBuilder.h"
#include "BaseRenderer\VertexBufferBuilder.h"

class Mesh2D : public IMesh2D
{
public:
	Mesh2D( );
	~Mesh2D( );

	// Lock/Unlock mesh
	virtual void LockMesh( Mesh2DType_t type ) override;
	virtual void UnlockMesh( ) override;

	// Add vertex position
	virtual void AddPositionFloat2( Vector2Df position ) override;

	// Add vertex uv
	virtual void AddVertexUVFloat2( Vector2Df uv ) override;

	// Add vertex color
	virtual void AddVertexColor( unsigned long color ) override;

	// Set color mesh
	virtual void SetMeshAsColor( bool state ) override;

	// Draw mesh
	virtual void DrawMesh( ) override;

	// Make render buffers
	void MakeRenderBuffers( );

protected:

	// Update vertex buffer
	void UpdateVertexBuffer( );

	// Update Solid Mesh
	void UpdateSolidMesh( );

	// Update mesh font
	void UpdateFontMesh( );

private:
	VertexBufferBuilder mVertexBufferBuilder;
	IndexBufferBuilder mIndexBufferBuilder;

	ID3D11Buffer* mVertexRenderBuffer;
	ID3D11Buffer* mIndexRenderBuffer;

	Mesh2DType_t mMeshType;

	bool mMeshHasColor;
};

#endif // !__RenderDx11_Mesh2D_h__
