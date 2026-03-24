/*

    Copyright 2024 Sergey Naumenkov

    File: IShaderSystem.h
    Description: Public interface for Shader System

    Date: 03/08/2024

*/

#ifndef __ShaderSystem_IShaderSystem_h__
#define __ShaderSystem_IShaderSystem_h__
#pragma once

#include <d3d11_1.h>
#include <vector>

#include "ShaderBundle.h"
#include "ShaderTypes.h"
#include "IShaderMatrix.h"

#include "ModuleLoader\IModule.h"

struct InputLayoutDesc
{
    const char* semanticName;  
    unsigned int semanticIndex;     
    unsigned int format;            
    unsigned int inputSlot;        
    unsigned int alignedByteOffset; 
};

class IShaderSystem : public IModule
{
public:

    // Load shader from file and compile
    virtual bool LoadShader( const char* file, ShaderType_e type, std::vector<InputLayoutDesc> inputLayout = { } ) = 0;

    // Save loaded shaders
    // Function must be call after loading all shaders.
    virtual void SaveShaders( const char* internalBundleName ) = 0;

    // Get shader by name
    virtual ShaderBundle_t* GetShadersByName( const char* internalBundleName ) = 0;

    // Bind shader texture
    virtual void BindTexture( int materialId, ShaderType_e type ) = 0;

    // Bind default sampler state
    virtual void BindDefaultSamplerState( ShaderType_e type ) = 0;

    // Bind shader bundle
    virtual void BindShaderBundle( ShaderBundle_t* bundle ) = 0;
    virtual void BindShaderBundle( const char* bundleName ) = 0;

    // Constant buffers zone
    
    // Allocate new constant buffer
    virtual int AllocateNewConstantBuffer( const char* bufferName, int size ) = 0;

    // Replace data in buffer
    virtual void UpdateConstantBufferData( ShaderType_e type, int bufferSlot, const char* bufferName, void* data, int size ) = 0;

    // Pop allocated buffer
    virtual void PopAllocatedBuffer( const char* bufferName ) = 0;

    // END 

    // get pointer on shader matrix class
    virtual IShaderMatrix* GetShaderMatrix() = 0;
};

#endif // !__ShaderSystem_IShaderSystem_h__
