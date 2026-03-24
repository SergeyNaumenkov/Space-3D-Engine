/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: Shader System for DirectX 11 renderer

	Date: 03/08/2024

*/

#include <vector>
#include "IShaderSystem.h"
#include "ShaderConstantBuffer.h"
#include "ShaderMatrix.h"

#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"
#include <DefaultShaderConstants.h>

#include "DirectX11Shader.h"

class ShaderSystem : public IShaderSystem
{
public:
	ShaderSystem( );
	~ShaderSystem( );

	// Pre initialize module
	virtual bool PreInit( ) override;

	// Initialize module
	virtual bool Initialize( ) override;

	// Start 
	virtual void Start( ) { }

	// Shutdown module
	virtual void Shutdown( ) override;

	// Load shader from file and compile
	virtual bool LoadShader( const char* file, ShaderType_e type, std::vector<InputLayoutDesc> inputLayout = { } );

	// Save loaded shaders
	// Function must be call after loading all shaders.
	virtual void SaveShaders( const char* internalBundleName );

	// Get shader by name
	virtual ShaderBundle_t* GetShadersByName( const char* internalBundleName );

	// Bind shader texture
	virtual void BindTexture( int materialId, ShaderType_e type );

	// Bind default sampler state
	virtual void BindDefaultSamplerState( ShaderType_e type );

	// Bind shader bundle
	virtual void BindShaderBundle( ShaderBundle_t* bundle );
	virtual void BindShaderBundle( const char* bundleName );

	// Allocate new constant buffer
	virtual int AllocateNewConstantBuffer( const char* bufferName, int size ) override;

	// Replace data in buffer
	virtual void UpdateConstantBufferData( ShaderType_e type, int bufferSlot, const char* bufferName, void* data, int size ) override;

	// Pop allocated buffer
	virtual void PopAllocatedBuffer( const char* bufferName ) override;

	// get pointer on shader matrix class
	virtual IShaderMatrix* GetShaderMatrix( );

private:
	std::vector<ShaderBundle_t*> mShaderBundles;
	ShaderBundle_t* mCurrentShaderBundle;

	ShaderConstantBuffer* mShaderConstants;
	ShaderMatrix* mShaderMatrix;

	DirectX11Shader* mDirectX11Shader;
};

/*
	Constructor
*/
ShaderSystem::ShaderSystem( )
{
	mCurrentShaderBundle = nullptr;
	mShaderConstants = nullptr;
	mShaderMatrix = nullptr;
	mDirectX11Shader = nullptr;
}

/*
	Destructor
*/
ShaderSystem::~ShaderSystem( )
{
	delete mCurrentShaderBundle;
	delete mShaderConstants;
	delete mShaderMatrix;
	delete mDirectX11Shader;
}

/*
	Pre initialize module
*/
bool ShaderSystem::PreInit( )
{
	// Load File System
	g_FileSystem = GetClassFromDll<IFileSystem*>( "FileSystem.dll", "IFileSystemV1" );
	if( !g_FileSystem )
	{
		return false;
	}

	// Load render system
	auto dll = LoadLibrary( "BaseRenderer.dll" );
	if( !dll )
		return false;

	typedef IBaseRenderer* ( *GetBaseRendererFunc )( const char* renderName );
	GetBaseRendererFunc func = ( GetBaseRendererFunc )GetProcAddress( dll, "GetBaseRenderer" );
	g_RenderSystem = func( "DirectX11" );
	FreeLibrary( dll );

	// Load Material System
	g_MaterialSystem = GetClassFromDll<IMaterialSystem*>( "MaterialSystem.dll", "IMaterialSystemV1" );
	if( !g_MaterialSystem )
	{
		PrintError( "Failed Get Material System!" );
		return false;
	}

	return true;
}

/*
	Initialize module
*/
bool ShaderSystem::Initialize( )
{
	// Shader constants
	mShaderConstants = new ShaderConstantBuffer( );

	// Shader matrices controller
	mShaderMatrix = new ShaderMatrix( );

	// Create default view,projection,world matrices
	mShaderMatrix->InitializeDefaultMatrices( );

	// D3d11 shader
	mDirectX11Shader = new DirectX11Shader( );

	return true;
}

/*
	Load shader from file and compile
*/
bool ShaderSystem::LoadShader( const char* file, ShaderType_e type, std::vector<InputLayoutDesc> inputLayout )
{
	// Create current shader bundle 
	if( !mCurrentShaderBundle )
		mCurrentShaderBundle = new ShaderBundle_t( );

	// Try load shader file
	if( !g_FileSystem->IsFileExist( file ) )
	{
		PrintError( "Failed load (%s) shader file!\nMaybe file is not found!", file );
		return false;
	}

	auto tag = g_RenderSystem->GetTag( );
	switch( tag )
	{
		case RendererTag_t::RenderTag_DirectX11:
			mDirectX11Shader->LoadShader( file, type, inputLayout, mCurrentShaderBundle );
			break;
		case RendererTag_t::RenderTag_OpenGL:
			break;
		case RendererTag_t::RenderTag_Metal:
			break;
		case RendererTag_t::RenderTag_Vulkan:
			break;
		default:
			break;
	}

	return true;
}

/*
	Save loaded shaders.
	Function must be call after loading all shaders.
*/
void ShaderSystem::SaveShaders( const char* internalBundleName )
{
	if( mCurrentShaderBundle )
	{
		mCurrentShaderBundle->mBundleName = internalBundleName;

		// Save it
		mShaderBundles.push_back( mCurrentShaderBundle );

		// Free current bundle
		mCurrentShaderBundle = nullptr;
	}
}

/*
	Get shader by name
*/
ShaderBundle_t* ShaderSystem::GetShadersByName( const char* internalBundleName )
{
	for( auto& bundle : mShaderBundles )
	{
		if( !strcmp( bundle->mBundleName, internalBundleName ) )
		{
			return bundle;
		}
	}

	PrintError( "Bundle with (%s) name is not found!", internalBundleName );
	return nullptr;
}

/*
	Bind shader texture
*/
void ShaderSystem::BindTexture( int materialId, ShaderType_e type )
{
	auto tag = g_RenderSystem->GetTag( );
	switch( tag )
	{
		case RendererTag_t::RenderTag_DirectX11:
			mDirectX11Shader->BindTexture( materialId, type );
			break;
		case RendererTag_t::RenderTag_OpenGL:
			break;
		case RendererTag_t::RenderTag_Metal:
			break;
		case RendererTag_t::RenderTag_Vulkan:
			break;
		default:
			break;
	}
}

/*
	Bind default sampler state
*/
void ShaderSystem::BindDefaultSamplerState( ShaderType_e type )
{
	auto tag = g_RenderSystem->GetTag( );
	switch( tag )
	{
		case RendererTag_t::RenderTag_DirectX11:
			mDirectX11Shader->BindDefaultSamplerState( type );
			break;
		case RendererTag_t::RenderTag_OpenGL:
			break;
		case RendererTag_t::RenderTag_Metal:
			break;
		case RendererTag_t::RenderTag_Vulkan:
			break;
		default:
			break;
	}
}

/*
	Bind shader bundle
*/
void ShaderSystem::BindShaderBundle( ShaderBundle_t* bundle )
{
	auto tag = g_RenderSystem->GetTag( );
	switch( tag )
	{
		case RendererTag_t::RenderTag_DirectX11:
			mDirectX11Shader->BindShaderBundle( bundle );
			break;
		case RendererTag_t::RenderTag_OpenGL:
			break;
		case RendererTag_t::RenderTag_Metal:
			break;
		case RendererTag_t::RenderTag_Vulkan:
			break;
		default:
			break;
	}
}

/*
	Bind shader bundle
*/
void ShaderSystem::BindShaderBundle( const char* bundleName )
{
	auto bundle = GetShadersByName( bundleName );
	if( !bundle )
		return;

	auto tag = g_RenderSystem->GetTag( );
	switch( tag )
	{
		case RendererTag_t::RenderTag_DirectX11:
			mDirectX11Shader->BindShaderBundle( bundle );
			break;
		case RendererTag_t::RenderTag_OpenGL:
			break;
		case RendererTag_t::RenderTag_Metal:
			break;
		case RendererTag_t::RenderTag_Vulkan:
			break;
		default:
			break;
	}
}

/*
	Shutdown module
*/
void ShaderSystem::Shutdown( )
{
	mShaderBundles.clear( );
	mShaderBundles.shrink_to_fit( );

	if( mCurrentShaderBundle )
	{
		delete mCurrentShaderBundle;
		mCurrentShaderBundle = nullptr;
	}
}

/*
	Allocate new constant buffer
*/
int ShaderSystem::AllocateNewConstantBuffer( const char* bufferName, int size )
{
	return mShaderConstants->AllocateNewBuffer( bufferName, size );
}

/*
	Replace data in buffer
*/
void ShaderSystem::UpdateConstantBufferData( ShaderType_e type, int bufferSlot, const char* bufferName, void* data, int size )
{
	mShaderConstants->UpdateBufferData( type, bufferSlot, bufferName, data, size );
}

/*
	Pop allocated buffer
*/
void ShaderSystem::PopAllocatedBuffer( const char* bufferName )
{
	mShaderConstants->PopAllocatedBuffer( bufferName );
}

/*
	get pointer on shader matrix class
*/
IShaderMatrix* ShaderSystem::GetShaderMatrix( )
{
	return mShaderMatrix;
}

SaveDllClass( ShaderSystem, IShaderSystemV1 )