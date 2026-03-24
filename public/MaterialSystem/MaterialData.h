/*

    Copyright 2024 Sergey Naumenkov

    File: MaterialData.h
    Description: Material data for loaded material from file

    Date: 03/08/2024

*/

#ifndef __MaterialSystem_MaterialData_h__
#define __MaterialSystem_MaterialData_h__
#pragma once

#include <d3d11_1.h>

// Material type 
// Maybe it's enum is not need
enum MaterialSurfaceType_e
{
    MatSurfaceType_None,
    MatSurfaceType_Air,

    MatSurfaceType_Grass,
    MatSurfaceType_Dirt,

    MatSurfaceType_Concrate,
    MatSurfaceType_Metal,

    MatSurfaceType_Water,
    MatSurfaceType_Sand,
    MatSurfaceType_Gravel,

    MatSurfaceType_Wood,
};

// For Storing a Diffuse, normal and etc textures
struct MaterialAdditionInformation_t
{
    // Diffuse Texture information
    const char* mDiffuseTextureFile;
    ID3D11ShaderResourceView* mDiffuseTexture;

    // Normal Texture information
    const char* mNormalTextureFile;
    ID3D11ShaderResourceView* mNormalTexture;

    // sound, when material is touched
    const char* mSoundFile;
};

// Material config
struct MaterialConfig_t
{
    const char* mConfigTag;

    MaterialSurfaceType_e* surfaceType;
    MaterialAdditionInformation_t* materialInformation;
};

// Main data for engine material
struct MaterialData_t
{
    // Path to file
    const char* mFile;

    // Material id
    int mMaterialID;

    // Material name
    const char* mMaterialName;

    // Bounds
    unsigned int mWidth;
    unsigned int mHeight;

    // Texture format
    DXGI_FORMAT mFormat;

    // Mip level
    unsigned int MipLevel;

    // Sample desc 
    DXGI_SAMPLE_DESC sample;

    // Pointer a texture
    void* mTexture;

    // Detailed information about material
    MaterialConfig_t* materialConfig;
};

#endif // !__MaterialSystem_MaterialData_h__
