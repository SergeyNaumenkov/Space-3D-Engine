/*

    Copyright 2024 Sergey Naumenkov

    File: ShaderConstantBuffer.h
    Description: Management constant buffers

    Date: 17/08/2024

*/

#ifndef __ShaderSystemDx11_ShaderConstantBuffer_h__
#define __ShaderSystemDx11_ShaderConstantBuffer_h__
#pragma once

#include <d3d11_1.h>
#include <vector>

#include "ShaderTypes.h"

struct ShaderConstantBuffer_t
{
    ID3D11Buffer* mBuffer;
    const char* mBufferName;
};

class ShaderConstantBuffer
{
public:
	ShaderConstantBuffer( );
	~ShaderConstantBuffer( );

    // Allocate new constant buffer
    int AllocateNewBuffer( const char* bufferName, int size );

    // Replace data in buffer
    void UpdateBufferData( ShaderType_e type, int bufferSlot, const char* bufferName, void* data, int size );

    // Pop allocated buffer
    void PopAllocatedBuffer( const char* bufferName );


private:
    std::vector<ShaderConstantBuffer_t> mShaderConstantBuffers;
};

#endif // !__ShaderSystemDx11_ShaderConstantBuffer_h__
