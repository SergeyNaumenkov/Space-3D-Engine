/*

	Copyright 2024 Sergey Naumenkov

	File: ShaderMatrix.cpp
	Description: Control of matrices shaders

	Date: 17/08/2024

*/

#include "ShaderMatrix.h"

#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"

/*
	Constructor
*/
ShaderMatrix::ShaderMatrix( )
{

}

/*
	Destructor
*/
ShaderMatrix::~ShaderMatrix( )
{

}

/*
	Initialize default matrices
	Call it once, in shader system initialize
*/
void ShaderMatrix::InitializeDefaultMatrices( )
{
	// View
	mShaderMatrices.push_back( XMMATRIX( ) );

	// Projection
	mShaderMatrices.push_back( XMMATRIX( ) );

	// World
	mShaderMatrices.push_back( XMMATRIX( ) );
}

/*
	Update default matrices
*/
void ShaderMatrix::UpdateDefaultMatrices( XMMATRIX view, XMMATRIX proj, XMMATRIX world )
{
	// Update view matrix
	mShaderMatrices[ 0 ] = view;

	// Update projection matrix
	mShaderMatrices[ 1 ] = proj;

	// Update world matrix
	mShaderMatrices[ 2 ] = view;
}

/*
	Update allocated matrix
*/
void ShaderMatrix::UpdateAllocatedMatrix( int matrixID, XMMATRIX matrix )
{
	if( matrixID < 3 )
	{
		PrintError( "You're can't update default matrix from this function!" );
		return;
	}

	if( matrixID > mShaderMatrices.size( ) - 1 )
		return;

	mShaderMatrices[ matrixID ] = matrix;
}

/*
	Allocate new matrix
	Return: index of new matrix
*/
int ShaderMatrix::AllocateMatrix( bool identity )
{
	XMMATRIX newMatrix;
	if( identity )
	{
		newMatrix = XMMatrixIdentity( );
	}

	// add
	mShaderMatrices.push_back( newMatrix );
	return static_cast< int >( mShaderMatrices.size( ) - 1 );
}

/*
	Get default matrix
*/
XMMATRIX ShaderMatrix::GetDefaultMatrix( DefaultShaderMatrix_e type )
{
	switch( type )
	{
		case DefaultShaderMatrix_e::Matrix_View:
			return mShaderMatrices[ 0 ];
		case DefaultShaderMatrix_e::Matrix_Projection:
			return mShaderMatrices[ 1 ];
		case DefaultShaderMatrix_e::Matrix_World:
			return mShaderMatrices[ 2 ];
		default:
			return mShaderMatrices[ 0 ];
			break;
	}
}

/*
	Get user allocated matrix
*/
XMMATRIX ShaderMatrix::GetAllocatedMatrix( int matrixID )
{
	if( matrixID < 3 )
	{
		PrintError( "You're can't get default matrix from this function!" );
		return {};
	}

	if( matrixID > mShaderMatrices.size( ) - 1 )
		return {};

	return mShaderMatrices[ matrixID ];
}