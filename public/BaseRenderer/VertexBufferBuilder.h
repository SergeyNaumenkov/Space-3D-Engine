/*

	Copyright 2024 Sergey Naumenkov

	File: VertexBufferBuilder.h
	Description: Shared vertex buffer builder

	Date: 25/08/2024

*/

#ifndef __BaseRenderer_VertexBufferBuilder_h__
#define __BaseRenderer_VertexBufferBuilder_h__
#pragma once

#include <vector>

#include "Math/Vector3D.h"
#include "Math/Vector2D.h"

#include "Core/Debug.h"

enum class VertexBufferBuilderType_t
{
	VB_For3D,
	VB_For2D
};

class VertexBufferBuilder
{
public:
	VertexBufferBuilder( );

	// Set vertex buffer type
	void SetBufferType( VertexBufferBuilderType_t type );

	// Lock buffer
	void Lock( );

	// Unlock buffer
	void Unlock( );

	// Add vertex position
	void AddPositionFloat3( Vector3Df position );
	void AddPositionFloat2( Vector2Df position );

	// Add vertex normal
	void AddNormalFloat3( Vector3Df normal );

	// Add vertex uv
	void AddUVFloat2( Vector2Df uv );

	// Add vertex color
	void AddColor( unsigned long color );

	// Get position
	const std::vector<Vector3Df>& GetVertexPositionFloat3( ) const;
	const std::vector<Vector2Df>& GetVertexPositionFloat2( ) const;

	// Get Normal vector
	const std::vector<Vector3Df>& GetVertexNormal( ) const;

	// Get uv
	const std::vector<Vector2Df>& GetVertexUV( ) const;

	// Get color
	const std::vector<unsigned long>& GetVertexColor( ) const;

	// Get max buffer size
	int GetMaxBufferSize( );

	// Prepare buffers
	bool Prepare( );

protected:

	// Clear buffer data
	void Reset( );

private:
	std::vector<Vector3Df> mVertexPositionFloat3;
	std::vector<Vector2Df> mVertexPositionFloat2;
	std::vector<Vector3Df> mVertexNormal;
	std::vector<Vector2Df> mVertexUV;
	std::vector<unsigned long> mVertexColor;

	int mVertexMaxSize;
	VertexBufferBuilderType_t mVertexBufferType;
};

/*
	Constructor
*/
inline VertexBufferBuilder::VertexBufferBuilder( )
{
	mVertexMaxSize = 2048;
	mVertexBufferType = VertexBufferBuilderType_t::VB_For3D;
}

/*
	Set vertex buffer type
*/
inline void VertexBufferBuilder::SetBufferType( VertexBufferBuilderType_t type )
{
	mVertexBufferType = type;
}

/*
	Prepare buffers
*/
inline bool VertexBufferBuilder::Prepare( )
{
	if( mVertexBufferType == VertexBufferBuilderType_t::VB_For3D )
	{
		mVertexPositionFloat3.reserve( mVertexMaxSize );
	}
	else
	{
		mVertexPositionFloat2.reserve( mVertexMaxSize );
	}

	mVertexUV.reserve( mVertexMaxSize );
	mVertexColor.reserve( mVertexMaxSize );
	mVertexNormal.reserve( mVertexMaxSize );

	return true;
}

/*
	Lock buffer
*/
inline void VertexBufferBuilder::Lock( )
{
	// Clears buffer data for new mesh
	Reset( );
}

/*
	Unlock buffer
*/
inline void VertexBufferBuilder::Unlock( )
{

}

/*
	Add vertex position
*/
inline void VertexBufferBuilder::AddPositionFloat3( Vector3Df position )
{
	if( mVertexBufferType != VertexBufferBuilderType_t::VB_For3D )
	{
		PrintError( "VertexBufferBuilder(AddPositionFloat3) -> Buffer must be prepared for 3D!" );
		return;
	}

	mVertexPositionFloat3.push_back( position );
}

/*
	Add vertex position float 2
*/
inline void VertexBufferBuilder::AddPositionFloat2( Vector2Df position )
{
	if( mVertexBufferType != VertexBufferBuilderType_t::VB_For2D )
	{
		PrintError( "VertexBufferBuilder(AddPositionFloat2) -> Buffer must be prepared for 2D!" );
		return;
	}

	mVertexPositionFloat2.push_back( position );
}

/*
	Add vertex normal
*/
inline void VertexBufferBuilder::AddNormalFloat3( Vector3Df normal )
{
	if( mVertexBufferType != VertexBufferBuilderType_t::VB_For3D )
	{
		PrintError( "VertexBufferBuilder(AddNormalFloat3) -> Buffer must be prepared for 3D!" );
		return;
	}

	mVertexNormal.push_back( normal );
}

/*
	Add vertex uv
*/
inline void VertexBufferBuilder::AddUVFloat2( Vector2Df uv )
{
	mVertexUV.push_back( uv );
}

/*
	Add vertex color
*/
inline void VertexBufferBuilder::AddColor( unsigned long color )
{
	mVertexColor.push_back( color );
}

/*
	Get position
*/
inline const std::vector<Vector3Df>& VertexBufferBuilder::GetVertexPositionFloat3( ) const
{
	return mVertexPositionFloat3;
}

/*
	Get vertex 3D position
*/
inline const std::vector<Vector2Df>& VertexBufferBuilder::GetVertexPositionFloat2( ) const
{
	return mVertexPositionFloat2;
}

/*
	Get Normal vector
*/
inline const std::vector<Vector3Df>& VertexBufferBuilder::GetVertexNormal( ) const
{
	return mVertexNormal;
}

/*
	Get uv
*/
inline const std::vector<Vector2Df>& VertexBufferBuilder::GetVertexUV( ) const
{
	return mVertexUV;
}

/*
	Get color
*/
inline const std::vector<unsigned long>& VertexBufferBuilder::GetVertexColor( ) const
{
	return mVertexColor;
}

/*
	Get max buffer size
*/
inline int VertexBufferBuilder::GetMaxBufferSize( )
{
	return mVertexMaxSize;
}

/*
	Clear buffer data
*/
inline void VertexBufferBuilder::Reset( )
{
	if( mVertexBufferType == VertexBufferBuilderType_t::VB_For3D )
	{
		// Only 3D
		mVertexPositionFloat3.clear( );
	}
	else
	{
		mVertexPositionFloat2.clear( );
	}

	// Shared buffers for 2D and 3D
	mVertexUV.clear( );
	mVertexColor.clear( );
	mVertexNormal.clear( );
}


#endif // !__BaseRenderer_VertexBufferBuilder_h__
