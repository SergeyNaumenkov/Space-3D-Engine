/*

	Copyright 2024 Sergey Naumenkov

	File: MapBox.cpp
	Description: Box primitive

	Date: 15/08/2024

*/

#include "MapBox.h"

#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"

const Vector2Df g_uv[ 24 ] =
{
	{1.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 1.0f},
	{1.0f, 1.0f},

	{0.0f, 0.0f},
	{1.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},
	{0.0f, 1.0f},
	{1.0f, 1.0f},
	{1.0f, 0.0f},
	{0.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},
	{0.0f, 0.0f},
	{1.0f, 0.0f},
	{0.0f, 1.0f},
	{1.0f, 1.0f},
	{1.0f, 0.0f},
	{0.0f, 0.0f},
	{1.0f, 1.0f},
	{0.0f, 1.0f},
	{0.0f, 0.0f},
	{1.0f, 0.0f},
};

/*
	Constructor
*/
MapSolidBox::MapSolidBox( )
{

}

/*
	Destructor
*/
MapSolidBox::~MapSolidBox( )
{

}

/*
	Create map box (solid)
*/
void MapSolidBox::CreateFromBoundingBox( BoundingBox_t& bbox )
{
	SolidBoxFace_t face = {};
	std::vector<Vector3Df> points( 4 );

	// Front
	// First triangle
	points[ 0 ].x = bbox.min.x; points[ 1 ].x = bbox.max.x;
	points[ 0 ].y = bbox.max.y; points[ 1 ].y = bbox.max.y;
	points[ 0 ].z = bbox.max.z; points[ 1 ].z = bbox.max.z;

	// Second triangle
	points[ 2 ].x = bbox.max.x; points[ 3 ].x = bbox.min.x;
	points[ 2 ].y = bbox.min.y; points[ 3 ].y = bbox.min.y;
	points[ 2 ].z = bbox.max.z; points[ 3 ].z = bbox.max.z;

	face.mVertex = points;
	face.mTotalVerticesCount = points.size( );

	face.mVertexUV.push_back( { g_uv[ 1 ] } );
	face.mVertexUV.push_back( { g_uv[ 0 ] } );
	face.mVertexUV.push_back( { g_uv[ 3 ] } );
	face.mVertexUV.push_back( { g_uv[ 2 ] } );

	face.normal = Vector3Df( 0, 0, 1 );

	mBoxFaces.push_back( face );

	// Back
// First triangle
	points[ 0 ].x = bbox.max.x; points[ 0 ].y = bbox.max.y; points[ 0 ].z = bbox.min.z;
	points[ 1 ].x = bbox.min.x; points[ 1 ].y = bbox.max.y; points[ 1 ].z = bbox.min.z;

	// Second triangle
	points[ 2 ].x = bbox.min.x; points[ 2 ].y = bbox.min.y; points[ 2 ].z = bbox.min.z;
	points[ 3 ].x = bbox.max.x; points[ 3 ].y = bbox.min.y; points[ 3 ].z = bbox.min.z;

	face.mVertex = points;
	face.mTotalVerticesCount = points.size( );

	face.normal = Vector3Df( 0, 0, -1 );

	mBoxFaces.push_back( face );

	// Left
// First triangle
	points[ 0 ].x = bbox.min.x; points[ 0 ].y = bbox.max.y; points[ 0 ].z = bbox.min.z;
	points[ 1 ].x = bbox.min.x; points[ 1 ].y = bbox.max.y; points[ 1 ].z = bbox.max.z;

	// Second triangle
	points[ 2 ].x = bbox.min.x; points[ 2 ].y = bbox.min.y; points[ 2 ].z = bbox.max.z;
	points[ 3 ].x = bbox.min.x; points[ 3 ].y = bbox.min.y; points[ 3 ].z = bbox.min.z;

	face.mVertex = points;
	face.mTotalVerticesCount = points.size( );

	face.normal = Vector3Df( -1, 0, 0 );

	mBoxFaces.push_back( face );

	// Right
// First triangle
	points[ 0 ].x = bbox.max.x; points[ 0 ].y = bbox.max.y; points[ 0 ].z = bbox.max.z;
	points[ 1 ].x = bbox.max.x; points[ 1 ].y = bbox.max.y; points[ 1 ].z = bbox.min.z;

	// Second triangle
	points[ 2 ].x = bbox.max.x; points[ 2 ].y = bbox.min.y; points[ 2 ].z = bbox.min.z;
	points[ 3 ].x = bbox.max.x; points[ 3 ].y = bbox.min.y; points[ 3 ].z = bbox.max.z;

	face.mVertex = points;
	face.mTotalVerticesCount = points.size( );

	face.normal = Vector3Df( 1, 0, 0 );

	mBoxFaces.push_back( face );

	// Top
// First triangle
	points[ 0 ].x = bbox.min.x; points[ 0 ].y = bbox.max.y; points[ 0 ].z = bbox.min.z;
	points[ 1 ].x = bbox.max.x; points[ 1 ].y = bbox.max.y; points[ 1 ].z = bbox.min.z;

	// Second triangle
	points[ 2 ].x = bbox.max.x; points[ 2 ].y = bbox.max.y; points[ 2 ].z = bbox.max.z;
	points[ 3 ].x = bbox.min.x; points[ 3 ].y = bbox.max.y; points[ 3 ].z = bbox.max.z;

	face.mVertex = points;
	face.mTotalVerticesCount = points.size( );

	face.normal = Vector3Df( 0, 1, 0 );

	mBoxFaces.push_back( face );

	// Bottom
// First triangle
	points[ 0 ].x = bbox.max.x; points[ 0 ].y = bbox.min.y; points[ 0 ].z = bbox.min.z;
	points[ 1 ].x = bbox.min.x; points[ 1 ].y = bbox.min.y; points[ 1 ].z = bbox.min.z;

	// Second triangle
	points[ 2 ].x = bbox.min.x; points[ 2 ].y = bbox.min.y; points[ 2 ].z = bbox.max.z;
	points[ 3 ].x = bbox.max.x; points[ 3 ].y = bbox.min.y; points[ 3 ].z = bbox.max.z;

	face.mVertex = points;
	face.mTotalVerticesCount = points.size( );

	face.mVertexUV.clear( );
	face.mVertexUV.push_back( { 0,0 } );
	face.mVertexUV.push_back( { 1,0 } );
	face.mVertexUV.push_back( { 1,1 } );
	face.mVertexUV.push_back( { 0,1 } );

	face.normal = Vector3Df( 0, -1, 0 );

	mBoxFaces.push_back( face );
}

/*
	Render solid faces
*/
void MapSolidBox::RenderSolidFaces( IMesh* mesh )
{
	if( !mesh )
		return;

	// Add each face to mesh buffer
	for( auto& face : mBoxFaces )
	{
		for( int i = 0; i < face.mTotalVerticesCount; i++ )
		{
			mesh->AddPositionFloat3( face.mVertex[ i ] );
			mesh->AddUVFloat2( face.mVertexUV[ i ] );
			mesh->AddNormalFloat3( face.normal );
		}
	}
}
