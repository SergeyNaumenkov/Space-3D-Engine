/*

    Copyright 2024 Sergey Naumenkov

    File: DirectX11Shader.cpp
    Description: Manager for DirectX 11 shaders

    Date: 25/08/2024

*/

#include "DirectX11Shader.h"

#include "InterfaceFactory\InterfaceFactory.h"
#include "Core\Debug.h"
#include "Core\TemplateConverter.h"

/*
    Constructor
*/
DirectX11Shader::DirectX11Shader( )
{
    mDefaultSamplerState = nullptr;
}

/*
    Destructor
*/
DirectX11Shader::~DirectX11Shader( )
{
    if( mDefaultSamplerState )
    {
        mDefaultSamplerState->Release( );
        mDefaultSamplerState = nullptr;
    }
}

/*
    Load shader
*/
bool DirectX11Shader::LoadShader( const char* file, ShaderType_e type, std::vector<InputLayoutDesc> inputLayout, ShaderBundle_t*& bundle )
{
    // Try load shader file
    if( !g_FileSystem->IsFileExist( file ) )
    {
        PrintError( "Failed load (%s) shader file!\nMaybe file is not found!" );
        return false;
    }

    // Open and read shader file
    long shaderBufferSize = 0;
    g_FileSystem->SetOpenMode( FileMode_t::File_OpenBinary );
    auto shaderBuffer = g_FileSystem->OpenAndReadFile( file, &shaderBufferSize );

    // Get Render Device
    auto renderDevice = reinterpret_cast< ID3D11Device1* >( g_RenderSystem->GetDevice( ) );
    if( !renderDevice )
    {
        PrintError( "Failed get Render Device from Render System!" );
        return false;
    }

    bool isDone = false;
    switch( type )
    {
        case ShaderType_Vertex:
            isDone = CreateVertexShader( renderDevice, shaderBuffer, shaderBufferSize, inputLayout, bundle );
            break;
        case ShaderType_Pixel:
            isDone = CreatePixelShader( renderDevice, shaderBuffer, shaderBufferSize, bundle );
            break;
        case ShaderType_Geometry:
            break;
        default:
            break;
    }

    return isDone;
}

/*
    Bind shader texture
*/
void DirectX11Shader::BindTexture( int materialId, ShaderType_e type )
{
    // Get material
    auto material = g_MaterialSystem->GetMaterial( materialId );
    if( !material )
    {
        PrintError( "Failed load material with (%i) index!", materialId );
        return;
    }

    // get render device
    auto device = reinterpret_cast< ID3D11Device1* >( g_RenderSystem->GetDevice( ) );
    ID3D11DeviceContext1* context = nullptr;
    device->GetImmediateContext1( &context );

    auto mat = ConvertToNewValue<ID3D11ShaderResourceView*>( material->mTexture );
    switch( type )
    {
        case ShaderType_Vertex:
            context->VSSetShaderResources( 0, 1, &mat );
            break;
        case ShaderType_Pixel:
            context->PSSetShaderResources( 0, 1, &mat );
            break;
        case ShaderType_Geometry:
            context->GSSetShaderResources( 0, 1, &mat );
            break;
        default:
            break;
    }
}

/*
    Bind default sampler state
*/
void DirectX11Shader::BindDefaultSamplerState( ShaderType_e type )
{
    // get render device
    auto device = reinterpret_cast< ID3D11Device1* >( g_RenderSystem->GetDevice( ) );
    if( !mDefaultSamplerState )
    {
        // Temp solution
        // Because in OnInit function 
        // Create default sampler state g_RenderSystem->GetDevice( ) return nullptr
        // Because Render system is not initialize yet
        D3D11_SAMPLER_DESC desc = { };
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.MinLOD = 0;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        device->CreateSamplerState( &desc, &mDefaultSamplerState );
    }

    ID3D11DeviceContext1* context = nullptr;
    device->GetImmediateContext1( &context );

    switch( type )
    {
        case ShaderType_Vertex:
            context->VSSetSamplers( 0, 1, &mDefaultSamplerState );
            break;
        case ShaderType_Pixel:
            context->PSSetSamplers( 0, 1, &mDefaultSamplerState );
            break;
        case ShaderType_Geometry:
            context->GSSetSamplers( 0, 1, &mDefaultSamplerState );
            break;
        default:
            break;
    }
}

/*
    Bind shader bundle
*/
void DirectX11Shader::BindShaderBundle( ShaderBundle_t* bundle )
{
    // get render device
    auto device = reinterpret_cast< ID3D11Device1* >( g_RenderSystem->GetDevice( ) );
    ID3D11DeviceContext1* context = nullptr;
    device->GetImmediateContext1( &context );

    // Vertex shader
    if( bundle->mVertexShader )
        context->VSSetShader( bundle->mVertexShader, 0, 0 );

    // Vertex layout
    if( bundle->mVertexLayout )
        context->IASetInputLayout( bundle->mVertexLayout );

    // Pixel shader
    if( bundle->mPixelShader )
        context->PSSetShader( bundle->mPixelShader, 0, 0 );

    // Geometry shader
    if( bundle->mGeometryShader )
        context->GSSetShader( bundle->mGeometryShader, 0, 0 );
}

/*
    Create Pixel shader from memory
*/
bool DirectX11Shader::CreatePixelShader( ID3D11Device1* device, const char* buffer, long bufferSize, ShaderBundle_t*& bundle )
{
    ID3D11PixelShader* pixelShader = nullptr;
    auto res = device->CreatePixelShader( buffer, bufferSize, nullptr, &pixelShader );
    if( FAILED( res ) )
    {
        PrintError( "Failed create Pixel Shader!" );
        return false;
    }

    // Save our pixel shader
    bundle->mPixelShader = pixelShader;

    return true;
}

/*
    CreateVertexShader
*/
bool DirectX11Shader::CreateVertexShader( ID3D11Device1* device, const char* buffer, long bufferSize, std::vector<InputLayoutDesc> inputLayout, ShaderBundle_t*& bundle )
{
    // Create shader
    ID3D11VertexShader* vertexShader = nullptr;
    ID3D11InputLayout* vertexInputLayout = nullptr;

    auto res = device->CreateVertexShader( buffer, bufferSize, nullptr, &vertexShader );
    if( FAILED( res ) )
    {
        PrintError( "Failed create Vertex Shader!" );
        return false;
    }

    // Save our vertex shader
    bundle->mVertexShader = vertexShader;

    // Create input layout
    if( inputLayout.size() != 0 )
    {
        D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[ inputLayout.size( ) ]( );
        for( int i = 0; i < inputLayout.size(); i++ )
        {
            auto in = inputLayout[ i ];
            desc[ i ].AlignedByteOffset = in.alignedByteOffset;
            desc[ i ].Format = static_cast< DXGI_FORMAT >( in.format );
            desc[ i ].InputSlot = in.inputSlot;
            desc[ i ].SemanticIndex = in.semanticIndex;
            desc[ i ].SemanticName = in.semanticName;
        }

        res = device->CreateInputLayout( desc, inputLayout.size( ), buffer, bufferSize, &vertexInputLayout );
        if( FAILED( res ) )
        {
            PrintError( "Failed create Vertex Input Layout!" );
            return false;
        }

        // Save our vertex layout 
        bundle->mVertexLayout = vertexInputLayout;
    }
    else
    {
        PrintError( "ShaderSystem->Dx11 = InputLayout has 0 size!" );
        return false;
    }

    return true;
}

