/*

    Copyright 2024 Sergey Naumenkov

    File: Mesh.cpp
    Description: Render mesh

    Date: 25/08/2024

*/

#include <iostream>

#include "Mesh.h"
#include "Math/ColorVertex.h"
#include "RenderDevicesInternal.h"
#include "Core/Debug.h"

Mesh::Mesh( )
{
    mVertexRenderBuffer = nullptr;
    mIndexRenderBuffer = nullptr;

    mMeshType = MeshType_t::Mesh_3D;
    mMeshRenderType = MeshRenderType_t::Render_Triangles;
}

Mesh::~Mesh( )
{
    if( mVertexRenderBuffer )
    {
        mVertexRenderBuffer->Release( );
        mVertexRenderBuffer = nullptr;
    }

    if( mIndexRenderBuffer )
    {
        mIndexRenderBuffer->Release( );
        mIndexRenderBuffer = nullptr;
    }
}

/*
    Lock mesh
*/
void Mesh::LockMesh( MeshType_t meshType, MeshRenderType_t renderType )
{
    mVertexBuffer.Lock( );
    mIndexBuffer.Lock( );
}

/*
    Add vertex position
*/
void Mesh::AddPositionFloat3( Vector3Df position )
{
    mVertexBuffer.AddPositionFloat3( position );
}

/*
    Add vertex uv
*/
void Mesh::AddUVFloat2( Vector2Df uv )
{
    mVertexBuffer.AddUVFloat2( uv );
}

/*
    Add vertex color
*/
void Mesh::AddColor( unsigned long color )
{
    mVertexBuffer.AddColor( color );
}

/*
    Add vertex normal
*/
void Mesh::AddNormalFloat3( Vector3Df normal )
{
    mVertexBuffer.AddNormalFloat3( normal );
}

/*
    Unlock mesh
*/
void Mesh::UnlockMesh( )
{
    mVertexBuffer.Unlock( );
    mIndexBuffer.Unlock( );
}

/*
    Draw mesh
*/
void Mesh::DrawMesh( )
{
    // First - generate indices for saved vertices data
    mIndexBuffer.GenerateIndices( mVertexBuffer.GetVertexPositionFloat3( ).size( ) );

    // Bind primitive topology
    auto devices = GetRenderDevicesInternal( );
    switch( mMeshRenderType )
    {
        case MeshRenderType_t::Render_Triangles:
            devices->mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
            break;
        case MeshRenderType_t::Render_Lines:
            devices->mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
            break;
        case MeshRenderType_t::Render_Points:
            devices->mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
            break;
        default:
            // Unknown type.
            // Set default primitive topology
            devices->mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
            break;
    }

    // Update index buffer 
    UpdateIndexBuffer( );

    // Update vertex buffer
    UpdateVertexBuffer( );
}

/*
    Create render buffer
*/
void Mesh::MakeRenderBuffer( )
{
    // Prepare vertex buffer
    mVertexBuffer.Prepare( );

    auto devices = GetRenderDevicesInternal( );

    D3D11_BUFFER_DESC bufferDesc = { };
    bufferDesc.ByteWidth = 2048;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

    devices->mDevice->CreateBuffer( &bufferDesc, nullptr, &mVertexRenderBuffer );

    bufferDesc = { };
    bufferDesc.ByteWidth = 2048;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

    devices->mDevice->CreateBuffer( &bufferDesc, nullptr, &mIndexRenderBuffer );
}

int totalDrawCallCounts = 0;
/*
    Update vertex buffer
*/
void Mesh::UpdateVertexBuffer( )
{
    auto devices = GetRenderDevicesInternal( );
    auto context = devices->mDeviceContext;

    // Lock render buffer
    D3D11_MAPPED_SUBRESOURCE map;
    context->Map( mVertexRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map );

    // Data
    auto vertexPosition = mVertexBuffer.GetVertexPositionFloat3( );
    auto vertexUV = mVertexBuffer.GetVertexUV( );
    auto vertexNormal = mVertexBuffer.GetVertexNormal( );

    auto data = reinterpret_cast< TexturedVertex_t* >( map.pData );
    auto maxVertexBufferSize = mVertexBuffer.GetMaxBufferSize( );

    int totalSize = 0;
    int startIndexLocation = 0;
    int baseVertexLocation = 0;

    for( int i = 0; i < mVertexBuffer.GetVertexPositionFloat3( ).size( ); i++ )
    {
        if( IsBufferOverflow( totalSize, sizeof( TexturedVertex_t ) ) )
        {
            // Draw part
            context->Unmap( mVertexRenderBuffer, 0 );

            UINT stride = sizeof( TexturedVertex_t );
            UINT offset = 0;
            context->IASetVertexBuffers( 0, 1, &mVertexRenderBuffer, &stride, &offset );

            // Calculate how many indices correspond to the current batch
            int indicesCount = mIndexBuffer.CalculateIndicesCountForBatch( startIndexLocation, baseVertexLocation, totalSize );

            // Draw current batch
            devices->mDeviceContext->DrawIndexed( indicesCount, startIndexLocation, baseVertexLocation );
            totalDrawCallCounts++;

            // Update for the next batch
            startIndexLocation += indicesCount;
            baseVertexLocation += totalSize;

            // Re-map buffer for the next part
            context->Map( mVertexRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map );

            // Bind next batch packet into gpu buffer
            data = reinterpret_cast< TexturedVertex_t* >( map.pData );
            totalSize = 0;
        }

        data[ totalSize ].position = vertexPosition[ i ];
        data[ totalSize ].uv = vertexUV[ i ];
        data[ totalSize ].normal = vertexNormal[ i ];

        totalSize++;
    }

    // Final draw for remaining vertices
    context->Unmap( mVertexRenderBuffer, 0 );

    UINT stride = sizeof( TexturedVertex_t );
    UINT offset = 0;
    context->IASetVertexBuffers( 0, 1, &mVertexRenderBuffer, &stride, &offset );

    // Draw last part
    int remainingIndicesCount = mIndexBuffer.CalculateIndicesCountForBatch( startIndexLocation, baseVertexLocation, totalSize );
    devices->mDeviceContext->DrawIndexed( remainingIndicesCount, startIndexLocation, baseVertexLocation );
    totalDrawCallCounts++;

    PrintMessage( "Total draw calls: (%i)!", totalDrawCallCounts );
    totalDrawCallCounts = 0;
}

/*
    Update index buffer
*/
void Mesh::UpdateIndexBuffer( )
{
    auto devices = GetRenderDevicesInternal( );
    auto context = devices->mDeviceContext;

    // Lock render buffer
    D3D11_MAPPED_SUBRESOURCE map;
    context->Map( mIndexRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map );

    // Data
    auto indexBufferData = mIndexBuffer.GetIndicesBuffer( );
    auto data = reinterpret_cast< unsigned short* >( map.pData );
    for( int i = 0; i < mIndexBuffer.GetIndicesBuffer( ).size( ); i++ )
    {
        data[ i ] = indexBufferData[ i ];
    }

    // Unlock render buffer
    context->Unmap( mIndexRenderBuffer, 0 );

    // Insert buffer
    context->IASetIndexBuffer( mIndexRenderBuffer, DXGI_FORMAT_R16_UINT, 0 );
}

/*
    Check buffer overflow
*/
bool Mesh::IsBufferOverflow( int currentVertexOffset, int gpuDataStructSize )
{
    if( currentVertexOffset * gpuDataStructSize >= mVertexBuffer.GetMaxBufferSize( ) )
        return true;

    return false;
}
