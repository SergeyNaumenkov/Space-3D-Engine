/*

	Copyright 2024 Sergey Naumenkov

	File: SolidObjectsController.cpp
	Description: Controller for all solid objects.
				 Stored it and render each frame.

	Date: 16/08/2024

*/

#include "SolidObjectsController.h"

#include "InterfaceFactory/InterfaceFactory.h"
#include "Math/ColorVertex.h"
#include "Core/Debug.h"
#include "ShaderSystem/DefaultShaderConstants.h"

static SolidObjectsController* g_singletonController;

/*
	Constructor
*/
SolidObjectsController::SolidObjectsController( )
{
	// prepare vector
	// Base count for solid objects = 1k
	mMapSolidObjects.resize( 1000 );
	mSolidObjectsCount = 0;
	mVerticesTotalOffset = 0;
	mIndicesTotalOffset = 0;

	mSolidObjectsConstantBufferID = 0;

	g_singletonController = this;
}

/*
	Destructor
*/
SolidObjectsController::~SolidObjectsController( )
{

}

// For test, maybe remove)
SolidObjectsController* SolidObjectsController::GetInstance( )
{
	if( !g_singletonController )
		g_singletonController = new SolidObjectsController( );

	return g_singletonController;
}

/*
	Initialize controller
*/
bool SolidObjectsController::Initialize( )
{
	mSolidObjectsConstantBufferID = g_ShaderSystem->AllocateNewConstantBuffer( "3DMeshConstantBuffer", sizeof( DefaultShaderConstantBuffer_t ) );

	// load colored shaders
	g_ShaderSystem->LoadShader( "shaders\\DefaultTexturedPixelShader.cso", ShaderType_Pixel );
	std::vector<InputLayoutDesc> lay;

	InputLayoutDesc l;
	l.alignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	l.format = 6; // DXGI_FORMAT_R32G32B32_FLOAT
	l.inputSlot = 0;
	l.semanticIndex = 0;
	l.semanticName = "POSITION";
	lay.push_back( l );

	l.semanticName = "NORMAL";
	lay.push_back( l );

	l.format = 16; // DXGI_FORMAT_R32G32_FLOAT
	l.semanticName = "TEXCOORD";
	lay.push_back( l );

	g_ShaderSystem->LoadShader( "shaders\\DefaultTexturedVertexShader.cso", ShaderType_Vertex, lay );
	g_ShaderSystem->SaveShaders( "TextureShaders" );

	return true;
}

/*
	add solid object
*/
void SolidObjectsController::AddSolidObject( IMapSolidObject* object )
{
	if( !object )
		return; // Skip object because equal nullptr

	if( mSolidObjectsCount >= mMapSolidObjects.size( ) )
	{
		mMapSolidObjects.resize( mMapSolidObjects.size( ) + 100 );
	}

	// Add 
	mMapSolidObjects[ mSolidObjectsCount++ ] = object;
}

/*
	Remove solid object
*/
void SolidObjectsController::RemoveSolidObject( IMapSolidObject* object )
{
	for( auto it = mMapSolidObjects.begin( ); it != mMapSolidObjects.end( ); )
	{
		if( *it == object )
		{
			delete* it;
			it = mMapSolidObjects.erase( it );

			// Done
			break;
		}
		else
		{
			++it;
		}
	}
}

/*
	Render solid objects
*/
void SolidObjectsController::RenderSolidObjects( Render3DCamera* camera )
{
	// Just call render for solid object
	// each object self control which face must me pass in render pipeline
	IMesh* mesh = g_RenderSystem->GetRenderMesh( );
	mesh->LockMesh( MeshType_t::Mesh_3D, MeshRenderType_t::Render_Triangles );

	for( int i = 0; i < mSolidObjectsCount; i++ )
	{
		DefaultShaderConstantBuffer_t defs = { };
		defs.view = camera->GetViewMatrix( );
		defs.projection = camera->GetProjectionMatrix( );
		defs.world = XMMatrixIdentity( );

		// Update default constant buffer
		g_ShaderSystem->UpdateConstantBufferData( ShaderType_Vertex, 0, "3DMeshConstantBuffer", &defs, sizeof( DefaultShaderConstantBuffer_t ) );

		// setup shaders
		g_ShaderSystem->BindTexture( 0, ShaderType_Pixel );
		g_ShaderSystem->BindDefaultSamplerState( ShaderType_Pixel );

		// Bind shaders
		g_ShaderSystem->BindShaderBundle( "TextureShaders" );

		auto object = mMapSolidObjects[ i ];
		object->RenderSolidFaces( mesh );
	}

	mesh->UnlockMesh( );
	mesh->DrawMesh( );
}

