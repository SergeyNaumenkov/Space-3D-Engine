/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: Entry point for mist graphics user interface
	Note: This library is wrapper for mist GUI controls. Just draws the quads, text and etc.
		  You're can use 'mGuiControls' with You're own gui wrapper.

	Date: 10/09/2024

*/

#include "Core/Debug.h"

#include "IMistGui.h"
#include "ColoredConstantBuffer.h"
#include "QuadBatch.h"
#include "Color.h"

#include "InterfaceFactory\InterfaceFactory.h"

class MistGui : public IMistGui
{
public:
	MistGui( );
	~MistGui( );

	// Initialize library
	virtual bool Initialize( );

	// Draw colored quad 
	virtual void DrawColorQuad( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadColor );

	// Draw textured quad 
	virtual void DrawTexturedQuad( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadOverlayColor );

	// Bind material
	virtual void BindMaterial( const char* materialName );
	virtual void BindMaterial( int materialID );

	// Draw all batched 2D geometry
	virtual void Render( );

protected:

	// Load shaders
	bool LoadShaders( );

	// Draw colored quads
	void DrawColorQuads( );

	// Draw textured quads
	void DrawTextureQuads( );

private:
	int mShaderMatrixID;
	MaterialData_t* mLastMaterial;
	bool mNeedUpdateConstantBuffer;

	QuadBatch* mQuadBatch;
};

__declspec( align( 16 ) )
struct testConstBuffer_t
{
	int isColored;
};

MistGui::MistGui( )
{
	mShaderMatrixID = 0;
	mQuadBatch = nullptr;
	mLastMaterial = nullptr;
	mNeedUpdateConstantBuffer = false;
}

MistGui::~MistGui( )
{
	if( mQuadBatch )
	{
		delete mQuadBatch;
		mQuadBatch = nullptr;
	}
}

/*
	Initialize library
*/
bool MistGui::Initialize( )
{
	// Load File System
	g_FileSystem = GetClassFromDll<IFileSystem*>( "FileSystem.dll", "IFileSystemV1" );
	if( !g_FileSystem )
	{
		PrintError( "Failed load file system!" );
		return false;
	}

	// Load Shader System
	g_ShaderSystem = GetClassFromDll<IShaderSystem*>( "ShaderSystem.dll", "IShaderSystemV1" );
	if( !g_ShaderSystem )
	{
		PrintError( "Failed Get Shader System!" );
		return false;
	}

	// Load Material System
	g_MaterialSystem = GetClassFromDll<IMaterialSystem*>( "MaterialSystem.dll", "IMaterialSystemV1" );
	if( !g_MaterialSystem )
	{
		PrintError( "Failed Get Material System!" );
		return false;
	}

	// Load shader system
	if( !LoadShaders( ) )
		return false;

	// Load render system
	auto dll = LoadLibrary( "BaseRenderer.dll" );
	if( !dll )
		return false;

	typedef IBaseRenderer* ( *GetBaseRendererFunc )( const char* renderName );
	GetBaseRendererFunc func = ( GetBaseRendererFunc )GetProcAddress( dll, "GetBaseRenderer" );
	g_RenderSystem = func( "DirectX11" );
	FreeLibrary( dll );


	// Allocate constant buffer
	g_ShaderSystem->AllocateNewConstantBuffer( "ColoredGuiConstantBuffer", sizeof( ColorGuiConstantBuffer_t ) );

	// Allocate matrix
	mShaderMatrixID = g_ShaderSystem->GetShaderMatrix( )->AllocateMatrix( true );

	// Prepare matrix
	auto matrix = XMMatrixOrthographicOffCenterRH( 0.0f, 1280.f, 720.f, 0.0f, 0.0f, 1.0f );
	g_ShaderSystem->GetShaderMatrix( )->UpdateAllocatedMatrix( mShaderMatrixID, matrix );

	// Update constant buffer
	ColorGuiConstantBuffer_t defs = { };
	defs.guiOrthoMatrix = XMMatrixTranspose( g_ShaderSystem->GetShaderMatrix( )->GetAllocatedMatrix( mShaderMatrixID ) );

	// Update constant buffer
	g_ShaderSystem->UpdateConstantBufferData( ShaderType_Vertex, 0, "ColoredGuiConstantBuffer", &defs, sizeof( ColorGuiConstantBuffer_t ) );

	// Initialize batch manager
	mQuadBatch = new QuadBatch( );

	return true;
}

/*
	Draw colored quad
*/
void MistGui::DrawColorQuad( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadColor )
{
	QuadColored_t quad = { };
	quad.validQuad = true;

	mQuadBatch->MakeQuadVertex( quadPosition, quadSize, quadColor, quad, QuadVertexId_e::Left_Top );
	mQuadBatch->MakeQuadVertex( quadPosition, quadSize, quadColor, quad, QuadVertexId_e::Right_Top );
	mQuadBatch->MakeQuadVertex( quadPosition, quadSize, quadColor, quad, QuadVertexId_e::Left_Bottom );
	mQuadBatch->MakeQuadVertex( quadPosition, quadSize, quadColor, quad, QuadVertexId_e::Right_Bottom );

	// Save quad
	mQuadBatch->AddColorQuad( quad );
}

/*
	Draw textured quad
*/
void MistGui::DrawTexturedQuad( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadOverlayColor )
{
	QuadTextured_t quad = { };
	quad.validQuad = true;

	mQuadBatch->MakeQuadVertex( quadPosition, quadSize, quadOverlayColor, quad, QuadVertexId_e::Left_Top );
	mQuadBatch->MakeQuadVertex( quadPosition, quadSize, quadOverlayColor, quad, QuadVertexId_e::Right_Top );
	mQuadBatch->MakeQuadVertex( quadPosition, quadSize, quadOverlayColor, quad, QuadVertexId_e::Left_Bottom );
	mQuadBatch->MakeQuadVertex( quadPosition, quadSize, quadOverlayColor, quad, QuadVertexId_e::Right_Bottom );

	// Save quad
	mQuadBatch->AddTexturedQuad( quad );
}

/*
	Bind material
*/
void MistGui::BindMaterial( const char* materialName )
{
	mLastMaterial = g_MaterialSystem->GetMaterial( materialName );
}

/*
	Bind material
*/
void MistGui::BindMaterial( int materialID )
{
	mLastMaterial = g_MaterialSystem->GetMaterial( materialID );
}

/*
	Draw all batched 2D geometry
*/
void MistGui::Render( )
{
	if( mNeedUpdateConstantBuffer )
	{
		ColorGuiConstantBuffer_t defs = { };
		defs.guiOrthoMatrix = XMMatrixTranspose( g_ShaderSystem->GetShaderMatrix( )->GetAllocatedMatrix( mShaderMatrixID ) );

		// Update constant buffer
		g_ShaderSystem->UpdateConstantBufferData( ShaderType_Vertex, 0, "ColoredGuiConstantBuffer", &defs, sizeof( ColorGuiConstantBuffer_t ) );

		mNeedUpdateConstantBuffer = false;
	}

	// Bind shaders
	g_ShaderSystem->BindShaderBundle( "GuiShaders" );

	// Color 
	DrawColorQuads( );

	// Texture
	DrawTextureQuads( );
}

/*
	Load shaders
*/
bool MistGui::LoadShaders( )
{
	// Load vertex shader 
	std::vector<InputLayoutDesc> lay;
	InputLayoutDesc l = {};

	l.alignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	l.format = 16; // DXGI_FORMAT_R32G32_FLOAT
	l.inputSlot = 0;
	l.semanticIndex = 0;
	l.semanticName = "POSITION";
	lay.push_back( l );

	l.format = 16; // DXGI_FORMAT_R32G32_FLOAT
	l.semanticName = "TEXCOORD";
	lay.push_back( l );

	l.format = 87; // DXGI_FORMAT_B8G8R8A8_UNORM
	l.semanticName = "COLOR";
	lay.push_back( l );

	auto isLoaded = g_ShaderSystem->LoadShader( "shaders\\GuiVertexShader.cso", ShaderType_Vertex, lay );
	if( !isLoaded )
		return false;

	// Load pixel shader
	isLoaded = g_ShaderSystem->LoadShader( "shaders\\GuiPixelShader.cso", ShaderType_Pixel );
	if( !isLoaded )
		return false;

	// Save shaders into memory
	g_ShaderSystem->SaveShaders( "GuiShaders" );

	// Allocate pixel constant buffer
	g_ShaderSystem->AllocateNewConstantBuffer( "GuiPixelConstantBuffer", sizeof( testConstBuffer_t ) );

	return true;
}

/*
	Draw colored quads
*/
void MistGui::DrawColorQuads( )
{
	if( mQuadBatch->GetQuadCount( 0 ) == 0 )
		return;

	testConstBuffer_t coloredFlag = { };
	coloredFlag.isColored = 1;

	g_ShaderSystem->UpdateConstantBufferData( ShaderType_Pixel, 0, "GuiPixelConstantBuffer", &coloredFlag, sizeof( testConstBuffer_t ) );

	// Bind sampler and texture
	g_ShaderSystem->BindDefaultSamplerState( ShaderType_Pixel );

	// Draw color quads
	auto mesh = g_RenderSystem->GetRenderMesh2D( );
	mesh->LockMesh( Mesh2DType_t::Mesh_Solid );

	for( int i = 0; i < mQuadBatch->GetQuadCount( 0 ); i++ )
	{
		auto quad = mQuadBatch->GetColorQuad( i );
		for( int j = 0; j < 4; j++ )
		{
			mesh->AddPositionFloat2( quad.quadVertex[ j ].vertexPosition );
			mesh->AddVertexColor( quad.quadVertex[ j ].vertexColor );
		}
	}

	mesh->UnlockMesh( );
	mesh->DrawMesh( );

	mQuadBatch->RemoveColorQuads( );
}

/*
	Draw textured quads
*/
void MistGui::DrawTextureQuads( )
{
	if( mQuadBatch->GetQuadCount( 1 ) == 0 )
		return;

	// Bind material
	if( mLastMaterial == nullptr )
	{
		return; // Skip, because material is not set
	}

	testConstBuffer_t texturedFlag = { };
	texturedFlag.isColored = 0;

	g_ShaderSystem->UpdateConstantBufferData( ShaderType_Pixel, 0, "GuiPixelConstantBuffer", &texturedFlag, sizeof( testConstBuffer_t ) );

	// Bind sampler and texture
	g_ShaderSystem->BindDefaultSamplerState( ShaderType_Pixel );
	g_ShaderSystem->BindTexture( mLastMaterial->mMaterialID, ShaderType_Pixel );

	// Draw textured quads
	auto mesh = g_RenderSystem->GetRenderMesh2D( );
	mesh->LockMesh( Mesh2DType_t::Mesh_Solid );

	for( int i = 0; i < mQuadBatch->GetQuadCount( 1 ); i++ )
	{
		auto quad = mQuadBatch->GetTexturedQuad( i );
		for( int j = 0; j < 4; j++ )
		{
			mesh->AddPositionFloat2( quad.quadVertex[ j ].vertexPosition );
			mesh->AddVertexUVFloat2( quad.quadVertex[ j ].uv );
			mesh->AddVertexColor( quad.quadVertex[ j ].vertexColor );
		}
	}

	mesh->UnlockMesh( );
	mesh->DrawMesh( );

	mQuadBatch->RemoveTexturedQuads( );
}

SaveDllClass( MistGui, IMistGuiV1 )