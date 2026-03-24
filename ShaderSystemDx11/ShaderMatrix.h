/*

    Copyright 2024 Sergey Naumenkov

    File: ShaderMatrix.h
    Description: Control of matrices shaders

    Date: 17/08/2024

*/

#ifndef __ShaderSystemDx11_ShaderMatrix_h__
#define __ShaderSystemDx11_ShaderMatrix_h__
#pragma once

#include <vector>

#include "IShaderMatrix.h"

class ShaderMatrix : public IShaderMatrix
{
public:
	ShaderMatrix( );
	~ShaderMatrix( );

    // Initialize default matrices
    // Call it once, when shader system initialize
    void InitializeDefaultMatrices( );

    // Update default matrices
    virtual void UpdateDefaultMatrices( XMMATRIX view, XMMATRIX proj, XMMATRIX world );

    // Update allocated matrix
    virtual void UpdateAllocatedMatrix( int matrixID, XMMATRIX matrix );

    // Allocate new matrix 
    // Return: index of new matrix
    virtual int AllocateMatrix( bool identity = true );

    // Get default matrix
    virtual XMMATRIX GetDefaultMatrix( DefaultShaderMatrix_e type );

    // Get user allocated matrix
    virtual XMMATRIX GetAllocatedMatrix( int matrixID );

private:
    std::vector<XMMATRIX> mShaderMatrices;
};

#endif // !__ShaderSystemDx11_ShaderMatrix_h__
