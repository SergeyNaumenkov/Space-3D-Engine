/*

	Copyright 2024 Sergey Naumenkov

	File: QuadBatch.cpp
	Description: Batch manager for quads

	Date: 10/09/2024

*/

#include "QuadBatch.h"

/*
	Constructor
*/
QuadBatch::QuadBatch( )
{

}

/*
	Add quad
*/
void QuadBatch::AddColorQuad( QuadColored_t quad )
{
	mColorQuads.AddToBack( quad );
}

/*
	Add quad 
*/
void QuadBatch::AddTexturedQuad( QuadTextured_t quad )
{
	mTextureQuads.AddToBack( quad );
}

/*
	Get quad count
*/
int QuadBatch::GetQuadCount( int quadType )
{
	if( quadType == 0 )
		return mColorQuads.size( );
	else if( quadType == 1 )
		return mTextureQuads.size( );
	else
		return 0;
}

/*
	Get quad
*/
QuadColored_t QuadBatch::GetColorQuad( int quadIndex )
{
	// If index is invalid return invalid quad 
	if( quadIndex > mColorQuads.size( ) - 1 || quadIndex < 0 )
	{
		QuadColored_t invalidQuad;
		invalidQuad.validQuad = false;

		return invalidQuad;
	}

	return mColorQuads[ quadIndex ];
}

/*
	Get quad
*/
QuadTextured_t QuadBatch::GetTexturedQuad( int quadIndex )
{
	// If index is invalid return invalid quad 
	if( quadIndex > mTextureQuads.size( ) - 1 || quadIndex < 0 )
	{
		QuadTextured_t invalidQuad;
		invalidQuad.validQuad = false;

		return invalidQuad;
	}

	return mTextureQuads[ quadIndex ];
}

/*
	Remove quads from collection
*/
void QuadBatch::RemoveColorQuads( )
{
	mColorQuads.ClearAll( );
}

/*
	Remove quads from collection
*/
void QuadBatch::RemoveTexturedQuads( )
{
	mTextureQuads.ClearAll( );
}

/*
	Make quad vertex
*/
void QuadBatch::MakeQuadVertex( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadColor, QuadColored_t& vertex, QuadVertexId_e type )
{
	switch( type )
	{
		case QuadVertexId_e::Left_Top:
			vertex.quadVertex[ 0 ].vertexColor = quadColor;
			vertex.quadVertex[ 0 ].vertexPosition.x = quadPosition.x;
			vertex.quadVertex[ 0 ].vertexPosition.y = quadPosition.y;
			break;
		case QuadVertexId_e::Right_Top:
			vertex.quadVertex[ 1 ].vertexColor = quadColor;
			vertex.quadVertex[ 1 ].vertexPosition.x = quadPosition.x + quadSize.x;
			vertex.quadVertex[ 1 ].vertexPosition.y = quadPosition.y;
			break;
		case QuadVertexId_e::Right_Bottom:
			vertex.quadVertex[ 2 ].vertexColor = quadColor;
			vertex.quadVertex[ 2 ].vertexPosition.x = quadPosition.x + quadSize.x;
			vertex.quadVertex[ 2 ].vertexPosition.y = quadPosition.y + quadSize.y;
			break;
		case QuadVertexId_e::Left_Bottom:
			vertex.quadVertex[ 3 ].vertexColor = quadColor;
			vertex.quadVertex[ 3 ].vertexPosition.x = quadPosition.x;
			vertex.quadVertex[ 3 ].vertexPosition.y = quadPosition.y + quadSize.y;
			break;
		default:
			break;
	}
}

/*
	Make quad vertex
*/
void QuadBatch::MakeQuadVertex( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadColor, QuadTextured_t& vertex, QuadVertexId_e type )
{
	switch( type )
	{
		case QuadVertexId_e::Left_Top:
			vertex.quadVertex[ 0 ].vertexColor = quadColor;
			vertex.quadVertex[ 0 ].vertexPosition.x = quadPosition.x;
			vertex.quadVertex[ 0 ].vertexPosition.y = quadPosition.y;
			vertex.quadVertex[ 0 ].uv = Vector2Df( 0, 0 );
			break;
		case QuadVertexId_e::Right_Top:
			vertex.quadVertex[ 1 ].vertexColor = quadColor;
			vertex.quadVertex[ 1 ].vertexPosition.x = quadPosition.x + quadSize.x;
			vertex.quadVertex[ 1 ].vertexPosition.y = quadPosition.y;
			vertex.quadVertex[ 1 ].uv = Vector2Df( 1, 0 );
			break;
		case QuadVertexId_e::Right_Bottom:
			vertex.quadVertex[ 2 ].vertexColor = quadColor;
			vertex.quadVertex[ 2 ].vertexPosition.x = quadPosition.x + quadSize.x;
			vertex.quadVertex[ 2 ].vertexPosition.y = quadPosition.y + quadSize.y;
			vertex.quadVertex[ 2 ].uv = Vector2Df( 1, 1 );
			break;
		case QuadVertexId_e::Left_Bottom:
			vertex.quadVertex[ 3 ].vertexColor = quadColor;
			vertex.quadVertex[ 3 ].vertexPosition.x = quadPosition.x;
			vertex.quadVertex[ 3 ].vertexPosition.y = quadPosition.y + quadSize.y;
			vertex.quadVertex[ 3 ].uv = Vector2Df( 0, 1 );
			break;
		default:
			break;
	}
}
