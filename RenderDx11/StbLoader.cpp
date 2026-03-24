/*

    Copyright 2024 Sergey Naumenkov

    File: StbLoader.cpp
    Description: Loads textures like the png jpg and etc with STB Image Loader

    Date: 14/09/2024

*/

#include "StbLoader.h"

// Interfaces:
#include "InterfaceFactory\InterfaceFactory.h"

// Core:
#include "Core\Debug.h"

// Stb Image Loader:
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
    Constructor
*/
StbLoader::StbLoader( )
{

}

/*
    Load material
*/
MaterialData_t* StbLoader::LoadMaterial( const char* materialPath )
{
    if( !g_FileSystem->IsFileExist( materialPath ) )
    {
        PrintError( "(%s) material is not exist! Failed load!" );
        return nullptr;
    }

    MaterialData_t* material = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;

    // Load image and get props of texture and buffer
    auto imageBuffer = stbi_load( materialPath, &width, &height, &channels, 4 );
    if( !imageBuffer )
    {
        PrintError( "Failed load image with STB Image Loader!" );
        return nullptr;
    }

    // Create texture from loaded data
    auto texture = CreateTexture( imageBuffer, width, height, 4 );
    if( !texture )
        return nullptr;

    material = new MaterialData_t( );
    material->mFile = "";
    material->mWidth = static_cast< unsigned int >( width );
    material->mHeight = static_cast< unsigned int >( width );
    material->mFormat = DXGI_FORMAT_R8_UNORM;
    material->mTexture = texture;

    return material;
}

/*
    Create texture
*/
ID3D11ShaderResourceView* StbLoader::CreateTexture( unsigned char* imageBuffer, int width, int height, int channelCount )
{
    D3D11_TEXTURE2D_DESC textureDesc = { };
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA textureData = { };
    textureData.pSysMem = imageBuffer;

    auto chCount = channelCount == 0 ? 1 : channelCount;
    textureData.SysMemPitch = static_cast< unsigned int >( width ) * chCount;

    // Create D3D11 texture 
    auto device = GetRenderDevicesInternal( )->mDevice;
    ID3D11Texture2D* texture = nullptr;

    auto res = device->CreateTexture2D( &textureDesc, &textureData, &texture );
    if( FAILED( res ) )
    {
        PrintError( "Failed create texture 2D!" );
        return nullptr;
    }

    // Create d3d11 shader resource view
    ID3D11ShaderResourceView* shaderResource = nullptr;
    res = device->CreateShaderResourceView( texture, nullptr, &shaderResource );
    if( FAILED( res ) )
    {
        PrintError( "Failed create shader resource view!" );
        return nullptr;
    }

    return shaderResource;
}
