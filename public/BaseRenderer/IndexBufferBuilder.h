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

class IndexBufferBuilder
{
public:
	IndexBufferBuilder( );

	// Lock buffer
	void Lock( );

	// Unlock buffer
	void Unlock( );

	// Get indices offset
	int CalculateIndicesCountForBatch( int startIndexLocation, int baseVertexLoc, int currentSize );

	// Generate indices
	void GenerateIndices( int VertexCount );

	// Get indices buffer
	std::vector<unsigned short> GetIndicesBuffer( );

private:
	std::vector<unsigned short> mBufferIndices;
};

/*
	Constructor
*/
inline IndexBufferBuilder::IndexBufferBuilder( )
{
	mBufferIndices.resize( 2048 );
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
	Get indices set
*/
inline int IndexBufferBuilder::CalculateIndicesCountForBatch( int startIndexLocation, int baseVertexLoc, int currentSize )
{
	int indicesCount = 0;

	for( int i = startIndexLocation; i < mBufferIndices.size( ); i++ )
	{
		if( mBufferIndices[ i ] < baseVertexLoc + currentSize )
		{
			indicesCount++;
		}
		else
		{
			break;
		}
	}

	return indicesCount;
}

/*
	Generate indices
*/
inline void IndexBufferBuilder::GenerateIndices( int VertexCount )
{
	unsigned short start = 0;
	auto total = VertexCount / 4;
	auto bufferOffset = mBufferIndices.size( );
	auto s = sizeof( unsigned short );
	for( unsigned short i = 0; i < total; i++ )
	{
		// Make first triangle
		mBufferIndices.push_back( start );
		mBufferIndices.push_back( start + 1 );
		mBufferIndices.push_back( start + 2 );

		// Make second triangle
		mBufferIndices.push_back( start );
		mBufferIndices.push_back( start + 2 );
		mBufferIndices.push_back( start + 3 );

		// Next
		start += 4;
	}
}

/*
	Get indices buffer
*/
inline std::vector<unsigned short> IndexBufferBuilder::GetIndicesBuffer( )
{
	return mBufferIndices;
}

#endif // !__BaseRenderer_IndexBufferBuilder_h__
