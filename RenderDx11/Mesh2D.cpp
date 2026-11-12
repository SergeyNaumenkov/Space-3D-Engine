/*

	Copyright 2024 Sergey Naumenkov

	File: Mesh2D.cpp
	Description: Render mesh 2D

	Date: 27/08/2024

*/

#include <algorithm>

#include "Mesh2D.h"
#include "RenderDevicesInternal.h"
#include "Math/ColorVertex.h"

/*
	Fill gpu buffer with gui vertices and draw them by batches.
	Uv and color are optional
*/
template<class GuiVertex>
static void DrawGuiVertices( ID3D11Buffer* vertexBuffer, const VertexBufferBuilder& vertices )
{
	auto devices = GetRenderDevicesInternal( );
	auto context = devices->mDeviceContext;

	const auto& positions = vertices.GetVertexPositionFloat2( );
	const auto& uvs = vertices.GetVertexUV( );
	const auto& colors = vertices.GetVertexColor( );

	UINT stride = sizeof( GuiVertex );
	UINT offset = 0;
	context->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );

	int verticesCount = static_cast< int >( positions.size( ) );
	for( int batchStart = 0; batchStart < verticesCount; batchStart += MaxBatchVertices )
	{
		int batchSize = ( std::min )( MaxBatchVertices, verticesCount - batchStart );

		// Lock render buffer
		D3D11_MAPPED_SUBRESOURCE map;
		if( FAILED( context->Map( vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map ) ) )
		{
			PrintError( "Mesh2D -> Failed map vertex buffer!" );
			return;
		}

		// Fill current batch
		auto gpuData = reinterpret_cast< GuiVertex* >( map.pData );
		for( int i = 0; i < batchSize; i++ )
		{
			size_t vertex = static_cast< size_t >( batchStart + i );

			gpuData[ i ].position = positions[ vertex ];
			gpuData[ i ].uv = vertex < uvs.size( ) ? uvs[ vertex ] : Vector2Df( );
			gpuData[ i ].color = vertex < colors.size( ) ? colors[ vertex ] : 0xFFFFFFFF;
		}

		// Unlock render buffer
		context->Unmap( vertexBuffer, 0 );

		// Draw current batch
		devices->DrawIndexed( IndexBufferBuilder::CalculateIndicesCountForBatch( batchSize ), 0, 0 );
	}
}

Mesh2D::Mesh2D( )
{
	mVertexRenderBuffer = nullptr;
	mIndexRenderBuffer = nullptr;

	mMeshHasColor = false;

	mMeshType = Mesh2DType_t::Mesh_Solid;
}

Mesh2D::~Mesh2D( )
{
	if( mIndexRenderBuffer )
	{
		mIndexRenderBuffer->Release( );
		mIndexRenderBuffer = nullptr;
	}

	if( mVertexRenderBuffer )
	{
		mVertexRenderBuffer->Release( );
		mVertexRenderBuffer = nullptr;
	}
}

/*
	Lock Mesh
*/
void Mesh2D::LockMesh( Mesh2DType_t type )
{
	mVertexBufferBuilder.Lock( );

	mMeshType = type;
}

/*
	Unlock mesh
*/
void Mesh2D::UnlockMesh( )
{
	mVertexBufferBuilder.Unlock( );
}

/*
	Add vertex position
*/
void Mesh2D::AddPositionFloat2( Vector2Df position )
{
	mVertexBufferBuilder.AddPositionFloat2( position );
}

/*
	Add vertex uv
*/
void Mesh2D::AddVertexUVFloat2( Vector2Df uv )
{
	mVertexBufferBuilder.AddUVFloat2( uv );
}

/*
	Add vertex color
*/
void Mesh2D::AddVertexColor( unsigned long color )
{
	mVertexBufferBuilder.AddColor( color );
}

/*
	Set color mesh
*/
void Mesh2D::SetMeshAsColor( bool state )
{
	mMeshHasColor = state;
}

/*
	Draw mesh
*/
void Mesh2D::DrawMesh( )
{
	if( !mVertexRenderBuffer || !mIndexRenderBuffer )
		return;

	// Bind primitive topology
	auto devices = GetRenderDevicesInternal( );
	devices->mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Bind index buffer
	devices->mDeviceContext->IASetIndexBuffer( mIndexRenderBuffer, DXGI_FORMAT_R16_UINT, 0 );

	// Upload vertices and draw them by batches
	UpdateVertexBuffer( );
}

/*
	Make render buffers
*/
void Mesh2D::MakeRenderBuffers( )
{
	// Redirect vertex buffer to 2D render
	mVertexBufferBuilder.SetBufferType( VertexBufferBuilderType_t::VB_For2D );

	// Prepare vertex buffer
	mVertexBufferBuilder.Prepare( );

	// Create render buffers
	auto devices = GetRenderDevicesInternal( );

	// Size for largest gui vertex
	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.ByteWidth = MaxBatchVertices * static_cast< UINT >( ( std::max )( sizeof( GuiSolidVertex_t ), sizeof( GuiSolidFontVertex_t ) ) );
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if( FAILED( devices->mDevice->CreateBuffer( &bufferDesc, nullptr, &mVertexRenderBuffer ) ) )
	{
		PrintError( "Mesh2D -> Failed create vertex buffer!" );
		return;
	}

	// Indices are local for batch, so they are generated once
	// and shared between all batches
	mIndexBufferBuilder.GenerateIndices( MaxBatchVertices );
	const auto& indices = mIndexBufferBuilder.GetIndicesBuffer( );

	bufferDesc = { };
	bufferDesc.ByteWidth = static_cast< UINT >( indices.size( ) * sizeof( unsigned short ) );
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData = { };
	indexData.pSysMem = indices.data( );

	if( FAILED( devices->mDevice->CreateBuffer( &bufferDesc, &indexData, &mIndexRenderBuffer ) ) )
	{
		PrintError( "Mesh2D -> Failed create index buffer!" );
		return;
	}
}

/*
	Update vertex buffer
*/
void Mesh2D::UpdateVertexBuffer( )
{
	switch( mMeshType )
	{
		case Mesh2DType_t::Mesh_Solid:
			UpdateSolidMesh( );
			break;
		case Mesh2DType_t::Mesh_Font:
			UpdateFontMesh( );
			break;
		default:
			break;
	}
}

/*
	Update Solid Mesh
*/
void Mesh2D::UpdateSolidMesh( )
{
	DrawGuiVertices<GuiSolidVertex_t>( mVertexRenderBuffer, mVertexBufferBuilder );
}

/*
	Update mesh font
*/
void Mesh2D::UpdateFontMesh( )
{
	DrawGuiVertices<GuiSolidFontVertex_t>( mVertexRenderBuffer, mVertexBufferBuilder );
}
