/*

	Copyright 2024 Sergey Naumenkov

	File: Mesh2D.cpp
	Description: Render mesh 2D

	Date: 27/08/2024

*/

#include "Mesh2D.h"
#include "RenderDevicesInternal.h"
#include "Math/ColorVertex.h"

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
	mIndexBufferBuilder.Lock( );

	mMeshType = type;
}

/*
	Unlock mesh
*/
void Mesh2D::UnlockMesh( )
{
	mVertexBufferBuilder.Unlock( );
	mIndexBufferBuilder.Unlock( );
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
	// First - generate indices for saved vertices data
	mIndexBufferBuilder.GenerateIndices( mVertexBufferBuilder.GetVertexPositionFloat2( ).size( ) );

	// Bind primitive topology
	auto devices = GetRenderDevicesInternal( );
	devices->mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Update index buffer 
	UpdateIndexBuffer( );

	// Update vertex buffer
	UpdateVertexBuffer( );
}

/*
	Make render buffers
*/
void Mesh2D::MakeRenderBuffers( )
{
	// Prepare vertex buffer
	mVertexBufferBuilder.Prepare( );

	// Redirect vertex buffer to 2D render
	mVertexBufferBuilder.SetBufferType( VertexBufferBuilderType_t::VB_For2D );

	// Create render buffers
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
	// Get render devices
	auto devices = GetRenderDevicesInternal( );
	auto context = devices->mDeviceContext;

	// Lock render buffer
	D3D11_MAPPED_SUBRESOURCE map;
	context->Map( mVertexRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map );

	// Get data
	auto gpuData = reinterpret_cast< GuiSolidVertex_t* >( map.pData );
	auto vertices = mVertexBufferBuilder.GetVertexPositionFloat2( );
	auto verticesUV = mVertexBufferBuilder.GetVertexUV( );
	auto verticesColor = mVertexBufferBuilder.GetVertexColor( );

	// Counters
	int totalSize = 0;
	int totalSize2 = 0;
	int startIndexLocation = 0;
	int baseVertexLocation = 0;

	// Fill the render buffer
	int verticesCount = static_cast< int >( mVertexBufferBuilder.GetVertexPositionFloat2( ).size( ) );
	int debugTotalIndices = 0;

	UINT stride = sizeof( GuiSolidVertex_t );
	UINT offset = 0;
	context->IASetVertexBuffers( 0, 1, &mVertexRenderBuffer, &stride, &offset );

	for( int i = 0; i < verticesCount; i++ )
	{
		if( IsBufferOverflow( totalSize, sizeof( GuiSolidVertex_t ) ) )
		{
			// Draw part
			context->Unmap( mVertexRenderBuffer, 0 );

			// Calculate how many indices correspond to the current batch
			int indicesCount = mIndexBufferBuilder.CalculateIndicesCountForBatch( startIndexLocation, baseVertexLocation, totalSize );
			devices->DrawIndexed( indicesCount,
								  0,
								  0 );

			debugTotalIndices += indicesCount;

			// Update for the next batch
			startIndexLocation += indicesCount;
			baseVertexLocation += totalSize;

			// Re-map buffer for the next part
			context->Map( mVertexRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map );

			// Bind next batch packet into gpu buffer
			gpuData = reinterpret_cast< GuiSolidVertex_t* >( map.pData );
			totalSize = 0;
		}

		gpuData[ totalSize ].position = vertices[ i ];
		if( verticesUV.size( ) > 0 )
		{
			gpuData[ totalSize ].uv = verticesUV[ i ];
		}
		gpuData[ totalSize ].color = verticesColor[ i ];

		totalSize++;
	}

	context->Unmap( mVertexRenderBuffer, 0 );

	int indicesCount = mIndexBufferBuilder.CalculateIndicesCountForBatch( startIndexLocation, baseVertexLocation, totalSize );
	debugTotalIndices += indicesCount;
	devices->DrawIndexed( indicesCount,
						  0,
						  0 );

	// Debug
	//devices->DrawInfoDrawCalls( );
}

/*
	Update mesh font
*/
void Mesh2D::UpdateFontMesh( )
{
	// Get render devices
	auto devices = GetRenderDevicesInternal( );
	auto context = devices->mDeviceContext;

	// Lock render buffer
	D3D11_MAPPED_SUBRESOURCE map;
	context->Map( mVertexRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map );

	// Get data
	auto gpuData = reinterpret_cast< GuiSolidFontVertex_t* >( map.pData );
	auto vertices = mVertexBufferBuilder.GetVertexPositionFloat2( );
	auto verticesUV = mVertexBufferBuilder.GetVertexUV( );
	auto verticesColor = mVertexBufferBuilder.GetVertexColor( );

	// Counters
	int totalSize = 0;
	int totalSize2 = 0;
	int startIndexLocation = 0;
	int baseVertexLocation = 0;

	// Fill the render buffer
	int verticesCount = static_cast< int >( mVertexBufferBuilder.GetVertexPositionFloat2( ).size( ) );
	int debugTotalIndices = 0;

	UINT stride = sizeof( GuiSolidFontVertex_t );
	UINT offset = 0;
	context->IASetVertexBuffers( 0, 1, &mVertexRenderBuffer, &stride, &offset );

	for( int i = 0; i < verticesCount; i++ )
	{
		if( IsBufferOverflow( totalSize, sizeof( GuiSolidFontVertex_t ) ) )
		{
			// Draw part
			context->Unmap( mVertexRenderBuffer, 0 );

			// Calculate how many indices correspond to the current batch
			int indicesCount = mIndexBufferBuilder.CalculateIndicesCountForBatch( startIndexLocation, baseVertexLocation, totalSize );
			devices->DrawIndexed( indicesCount,
								  0,
								  0 );

			debugTotalIndices += indicesCount;

			// Update for the next batch
			startIndexLocation += indicesCount;
			baseVertexLocation += totalSize;

			// Re-map buffer for the next part
			context->Map( mVertexRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map );

			// Bind next batch packet into gpu buffer
			gpuData = reinterpret_cast< GuiSolidFontVertex_t* >( map.pData );
			totalSize = 0;
		}

		gpuData[ totalSize ].position = vertices[ i ];
		gpuData[ totalSize ].uv = verticesUV[ i ];
		gpuData[ totalSize ].color = verticesColor[ i ];

		totalSize++;
	}

	context->Unmap( mVertexRenderBuffer, 0 );

	int indicesCount = mIndexBufferBuilder.CalculateIndicesCountForBatch( startIndexLocation, baseVertexLocation, totalSize );
	debugTotalIndices += indicesCount;
	devices->DrawIndexed( indicesCount,
						  0,
						  0 );

	// Debug
	//devices->DrawInfoDrawCalls( );
}

/*
	Update index buffer
*/
void Mesh2D::UpdateIndexBuffer( )
{
	auto devices = GetRenderDevicesInternal( );
	auto context = devices->mDeviceContext;

	// Lock render buffer
	D3D11_MAPPED_SUBRESOURCE map;
	context->Map( mIndexRenderBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map );

	// Data
	auto indexBufferData = mIndexBufferBuilder.GetIndicesBuffer( );
	auto data = reinterpret_cast< unsigned short* >( map.pData );
	for( int i = 0; i < mIndexBufferBuilder.GetIndicesBuffer( ).size( ); i++ )
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
bool Mesh2D::IsBufferOverflow( int currentVertexOffset, int gpuDataStructSize )
{
	if( currentVertexOffset * gpuDataStructSize >= mVertexBufferBuilder.GetMaxBufferSize( ) )
		return true;

	return false;
}
