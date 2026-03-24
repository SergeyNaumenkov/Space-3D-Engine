/*

	Copyright 2024 Sergey Naumenkov

	File: QuadBatch.h
	Description: Batch manager for quads

	Date: 10/09/2024

*/

#ifndef __mGui_QuadBatch_h__
#define __mGui_QuadBatch_h__
#pragma once

#include "Math\Vector2D.h"
#include "Core\Vector.h"

enum class QuadVertexId_e
{
	Left_Top,
	Right_Top,
	Right_Bottom,
	Left_Bottom,
};

struct QuadVertex_t
{
	Vector2Df vertexPosition;
	Vector2Df uv;
	unsigned long vertexColor;
};

struct QuadColored_t
{
	QuadVertex_t quadVertex[ 4 ];
	bool validQuad;
};

struct QuadTextured_t
{
	QuadVertex_t quadVertex[ 4 ];
	bool validQuad;
};

class QuadBatch
{
public:
	QuadBatch( );

	// Add quad 
	void AddColorQuad( QuadColored_t quad );
	void AddTexturedQuad( QuadTextured_t quad );

	// Get quad count
	int GetQuadCount( int quadType );

	// Get quad
	QuadColored_t GetColorQuad( int quadIndex );
	QuadTextured_t GetTexturedQuad( int quadIndex );

	// Remove quads from collection
	void RemoveColorQuads( );
	void RemoveTexturedQuads( );

	// Make quad vertex
	void MakeQuadVertex( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadColor, QuadColored_t& vertex, QuadVertexId_e type );
	void MakeQuadVertex( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadColor, QuadTextured_t& vertex, QuadVertexId_e type );

private:
	Vector<QuadColored_t> mColorQuads;
	Vector<QuadTextured_t> mTextureQuads;
};

#endif // !__mGui_QuadBatch_h__
