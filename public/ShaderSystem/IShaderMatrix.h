/*

	Copyright 2024 Sergey Naumenkov

	File: IShaderMatrix.h
	Description: Public interface for shader matrices

	Date: 17/08/2024

*/

#ifndef __ShaderSystemDx11_IShaderMatrix_h__
#define __ShaderSystemDx11_IShaderMatrix_h__
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

enum class DefaultShaderMatrix_e
{
	Matrix_View,
	Matrix_Projection,
	Matrix_World
};

class IShaderMatrix
{
public:
	// Update default matrices
	virtual void UpdateDefaultMatrices( XMMATRIX view, XMMATRIX proj, XMMATRIX world ) = 0;

	// Update allocated matrix
	virtual void UpdateAllocatedMatrix( int matrixID, XMMATRIX matrix ) = 0;

	// Allocate new matrix 
	// Return: index of new matrix
	virtual int AllocateMatrix( bool identity = true ) = 0;

	// Get default matrix
	virtual XMMATRIX GetDefaultMatrix( DefaultShaderMatrix_e type ) = 0;

	// Get user allocated matrix
	virtual XMMATRIX GetAllocatedMatrix( int matrixID ) = 0;

};
#endif // !__ShaderSystemDx11_IShaderMatrix_h__
