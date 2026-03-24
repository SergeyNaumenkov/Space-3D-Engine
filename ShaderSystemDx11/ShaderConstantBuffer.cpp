/*

    Copyright 2024 Sergey Naumenkov

    File: ShaderConstantBuffer.cpp
    Description: Management constant buffers

    Date: 17/08/2024

*/

#include "ShaderConstantBuffer.h"

#include "DefaultShaderConstants.h"

#include "InterfaceFactory/InterfaceFactory.h"
#include "Core/Debug.h"
#include "Core/StringTools.h"

/*
    Constructor
*/
ShaderConstantBuffer::ShaderConstantBuffer( )
{
}

/*
    Destructor
*/
ShaderConstantBuffer::~ShaderConstantBuffer( )
{
}

/*
    Allocate new constant buffer
*/
int ShaderConstantBuffer::AllocateNewBuffer( const char* bufferName, int size )
{
    // Maybe RenderSystem must allocate new buffer ?
    auto device = reinterpret_cast< ID3D11Device* >( g_RenderSystem->GetDevice( ) );
    ID3D11Buffer* constantBuffer = nullptr;

    D3D11_BUFFER_DESC bufferDesc = { };
    bufferDesc.ByteWidth = size;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    auto res = device->CreateBuffer( &bufferDesc, nullptr, &constantBuffer );
    if( FAILED( res ) )
    {
        PrintMessage( "Failed create constant buffer!" );
        return -1;
    }

    ShaderConstantBuffer_t buffer = { };
    buffer.mBuffer = constantBuffer;
    buffer.mBufferName = bufferName;

    mShaderConstantBuffers.push_back( buffer );
    return static_cast< int >( mShaderConstantBuffers.size( ) - 1 );
}

/*
    Replace data in buffer
*/
void ShaderConstantBuffer::UpdateBufferData( ShaderType_e type, int bufferSlot, const char* bufferName, void* data, int size )
{
    if( !bufferName )
        return;

    // get render device
    auto device = reinterpret_cast< ID3D11Device1* >( g_RenderSystem->GetDevice( ) );
    ID3D11DeviceContext1* context = nullptr;
    device->GetImmediateContext1( &context );

    // Try find the buffer
    ID3D11Buffer* buffer = nullptr;
    for( int i = 0; i < mShaderConstantBuffers.size(); i++ )
    {
        if( CompareStrings( mShaderConstantBuffers[ i ].mBufferName, bufferName ) )
        {
            buffer = mShaderConstantBuffers[ i ].mBuffer;
            break;
        }
    }

    if( !buffer )
        return;

    D3D11_MAPPED_SUBRESOURCE map;
    context->Map( buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map );

    // Update data
    memcpy( map.pData, data, size );

    context->Unmap( buffer, 0 );

    switch( type )
    {
        case ShaderType_Vertex:
            context->VSSetConstantBuffers( bufferSlot, 1, &buffer );
            break;
        case ShaderType_Pixel:
            context->PSSetConstantBuffers( bufferSlot, 1, &buffer );
            break;
        case ShaderType_Geometry:
            context->GSSetConstantBuffers( bufferSlot, 1, &buffer );
            break;
        default:
            break;
    }
}

/*
    Pop allocated buffer
*/
void ShaderConstantBuffer::PopAllocatedBuffer( const char* bufferName )
{

}