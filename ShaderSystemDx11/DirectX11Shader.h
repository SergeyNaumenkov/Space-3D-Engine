/*

    Copyright 2024 Sergey Naumenkov

    File: DirectX11Shader.h
    Description: Manager for DirectX 11 shaders

    Date: 25/08/2024

*/

#ifndef __ShaderSystem_DirectX11Shader_h__
#define __ShaderSystem_DirectX11Shader_h__
#pragma once

#include "IShaderSystem.h"

class DirectX11Shader
{
public:
	DirectX11Shader( );
	~DirectX11Shader( );

    // Load shader
    bool LoadShader( const char* file, ShaderType_e type, std::vector<InputLayoutDesc> inputLayout, ShaderBundle_t*& bundle );

    // Bind shader texture
    void BindTexture( int materialId, ShaderType_e type );

    // Bind default sampler state
    void BindDefaultSamplerState( ShaderType_e type );

    // Bind shader bundle
    virtual void BindShaderBundle( ShaderBundle_t* bundle );

protected:

    // Create Pixel shader from memory
    bool CreatePixelShader( ID3D11Device1* device, const char* buffer, long bufferSize, ShaderBundle_t*& bundle );

    // Create Vertex Shader
    bool CreateVertexShader( ID3D11Device1* device, const char* buffer, long bufferSize, std::vector<InputLayoutDesc> inputLayout, ShaderBundle_t*& bundle );

private:
    ID3D11SamplerState* mDefaultSamplerState;
};

#endif // !__ShaderSystem_DirectX11Shader_h__
