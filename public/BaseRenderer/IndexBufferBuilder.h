/*

	Copyright 2024 Sergey Naumenkov

	File: IndexBufferBuilder.h
	Description: Shared index buffer builder

	Date: 25/08/2024

*/

#ifndef __BaseRenderer_IndexBufferBuilder_h__
#define __BaseRenderer_IndexBufferBuilder_h__
#pragma once

#include <vector>

// Vertices and indices count for one quad
constexpr int VerticesPerQuad = 4;
constexpr int IndicesPerQuad = 6;

class IndexBufferBuilder
{
public:
	IndexBufferBuilder( );

	// Lock buffer
	void Lock( );

	// Unlock buffer
	void Unlock( );

	// Get indices count for batch with given vertices count
	static int CalculateIndicesCountForBatch( int verticesCount );

	// Generate quad indices for one batch.
	// Indices are local for batch ( 0..VertexCount-1 ),
	// so one generated buffer is shared between all batches
	void GenerateIndices( int VertexCount );

	// Get indices buffer
	const std::vector<unsigned short>& GetIndicesBuffer( ) const;

private:
	std::vector<unsigned short> mBufferIndices;
};

/*
	Constructor
*/
inline IndexBufferBuilder::IndexBufferBuilder( )
{
	mBufferIndices.reserve( 2048 );
}

/*
	Lock buffer
*/
inline void IndexBufferBuilder::Lock( )
{
	// Reset data
	mBufferIndices.clear( );
}

/*
	Unlock buffer
*/
inline void IndexBufferBuilder::Unlock( )
{

}

/*
	Get indices count for batch with given vertices count
*/
inline int IndexBufferBuilder::CalculateIndicesCountForBatch( int verticesCount )
{
	// Only full quads are drawn
	return ( verticesCount / VerticesPerQuad ) * IndicesPerQuad;
}

/*
	Generate indices
*/
inline void IndexBufferBuilder::GenerateIndices( int VertexCount )
{
	// Indices are stored as 16-bit values
	if( VertexCount > 0xFFFF + 1 )
		VertexCount = 0xFFFF + 1;

	mBufferIndices.clear( );

	unsigned int start = 0;
	auto total = VertexCount / VerticesPerQuad;
	for( int i = 0; i < total; i++ )
	{
		// Make first triangle
		mBufferIndices.push_back( static_cast< unsigned short >( start ) );
		mBufferIndices.push_back( static_cast< unsigned short >( start + 1 ) );
		mBufferIndices.push_back( static_cast< unsigned short >( start + 2 ) );

		// Make second triangle
		mBufferIndices.push_back( static_cast< unsigned short >( start ) );
		mBufferIndices.push_back( static_cast< unsigned short >( start + 2 ) );
		mBufferIndices.push_back( static_cast< unsigned short >( start + 3 ) );

		// Next
		start += VerticesPerQuad;
	}
}

/*
	Get indices buffer
*/
inline const std::vector<unsigned short>& IndexBufferBuilder::GetIndicesBuffer( ) const
{
	return mBufferIndices;
}

#endif // !__BaseRenderer_IndexBufferBuilder_h__
