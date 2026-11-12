/*

    Copyright 2024 Sergey Naumenkov

    File: Mesh.cpp
    Description: Render mesh

    Date: 25/08/2024

*/

#include <algorithm>

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

    mMeshType = meshType;
    mMeshRenderType = renderType;
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
}

/*
    Draw mesh
*/
void Mesh::DrawMesh( )
{
    if( !mVertexRenderBuffer || !mIndexRenderBuffer )
        return;

    // Bind primitive topology
    auto devices = GetRenderDevicesInternal( );
    auto context = devices->mDeviceContext;
    switch( mMeshRenderType )
    {
        case MeshRenderType_t::Render_Triangles:
            context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
            break;
        case MeshRenderType_t::Render_Lines:
            context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
            break;
        case MeshRenderType_t::Render_Points:
            context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
            break;
        default:
            // Unknown type.
            // Set default primitive topology
            context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
            break;
    }

    // Bind render buffers
    UINT stride = sizeof( TexturedVertex_t );
    UINT offset = 0;
    context->IASetVertexBuffers( 0, 1, &mVertexRenderBuffer, &stride, &offset );
    context->IASetIndexBuffer( mIndexRenderBuffer, DXGI_FORMAT_R16_UINT, 0 );

    // Upload vertices and draw them by batches
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
    bufferDesc.ByteWidth = MaxBatchVertices * sizeof( TexturedVertex_t );
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if( FAILED( devices->mDevice->CreateBuffer( &bufferDesc, nullptr, &mVertexRenderBuffer ) ) )
    {
        PrintError( "Mesh -> Failed create vertex buffer!" );
        return;
    }

    // Indices are local for batch, so they are generated once
    // and shared between all batches
    mIndexBuffer.GenerateIndices( MaxBatchVertices );
    const auto& indices = mIndexBuffer.GetIndicesBuffer( );

    bufferDesc = { };
    bufferDesc.ByteWidth = static_cast< UINT >( indices.size( ) * sizeof( unsigned short ) );
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = { };
    indexData.pSysMem = indices.data( );

    if( FAILED( devices->mDevice->CreateBuffer( &bufferDesc, &indexData, &mIndexRenderBuffer ) ) )
    {
        PrintError( "Mesh -> Failed create index buffer!" );
        return;
    }
}

/*
    Update vertex buffer
*/
void Mesh::UpdateVertexBuffer( )
{
    auto devices = GetRenderDevicesInternal( );
    auto context = devices->mDeviceContext;

    // Data
    const auto& vertexPosition = mVertexBuffer.GetVertexPositionFloat3( );
    const auto& vertexUV = mVertexBuffer.GetVertexUV( );
    const auto& vertexNormal = mVertexBuffer.GetVertexNormal( );

    int verticesCount = static_cast< int >( vertexPosition.size( ) );
    for( int batchStart = 0; batchStart < verticesCount; batchStart += MaxBatchVertices )
    {
        int batchSize = ( std::min )( MaxBatchVertices, verticesCount - batchStart );

        // Lock render buffer
        D3D11_MAPPED_SUBRESOURCE map;
        if( FAILED( context->Map( mVertexRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map ) ) )
        {
            PrintError( "Mesh -> Failed map vertex buffer!" );
            return;
        }

        // Fill current batch. Uv and normal are optional
        auto data = reinterpret_cast< TexturedVertex_t* >( map.pData );
        for( int i = 0; i < batchSize; i++ )
        {
            size_t vertex = static_cast< size_t >( batchStart + i );

            data[ i ].position = vertexPosition[ vertex ];
            data[ i ].uv = vertex < vertexUV.size( ) ? vertexUV[ vertex ] : Vector2Df( );
            data[ i ].normal = vertex < vertexNormal.size( ) ? vertexNormal[ vertex ] : Vector3Df( );
        }

        // Unlock render buffer
        context->Unmap( mVertexRenderBuffer, 0 );

        // Draw current batch
        DrawBatch( batchSize );
    }
}

/*
    Draw batch from vertex buffer
*/
void Mesh::DrawBatch( int verticesCount )
{
    auto devices = GetRenderDevicesInternal( );

    // Triangles are built from quads by index buffer,
    // lines and points are drawn as is
    if( mMeshRenderType == MeshRenderType_t::Render_Lines || mMeshRenderType == MeshRenderType_t::Render_Points )
        devices->Draw( verticesCount, 0 );
    else
        devices->DrawIndexed( IndexBufferBuilder::CalculateIndicesCountForBatch( verticesCount ), 0, 0 );
}
