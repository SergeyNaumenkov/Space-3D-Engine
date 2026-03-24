/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: Material System

	Date: 03/08/2024

*/

#include <vector>
#include "IMaterialSystem.h"

#include "Core/Debug.h"
#include "Core/StringTools.h"
#include "InterfaceFactory/InterfaceFactory.h"

class MaterialSystem : public IMaterialSystem
{
public:
	MaterialSystem( );
	~MaterialSystem( );

	// Load material from file
	virtual int LoadMaterial( const char* materialPath, const char* materialName );

	// Create material from memory
	virtual bool CreateMaterialFromMemory( const char* materialName, int width, int height, void* data, int offset );

	// Get Material by index
	virtual MaterialData_t* GetMaterial( int index );

	// Get Material by name or path
	virtual MaterialData_t* GetMaterial( const char* materialName, const char* materialPath = nullptr );

	// Release material
	virtual bool ReleaseMaterial( const char* materialName ) override;

	// Pre initialize module
	virtual bool PreInit( ) override;

	// Initialize module
	virtual bool Initialize( ) override;

	// Start 
	virtual void Start( ) { }

	// Shutdown module
	virtual void Shutdown( ) override;

protected:

private:
	std::vector<MaterialData_t*> mMaterials;
};

/*
	Constructor
*/
MaterialSystem::MaterialSystem( )
{
	
}

/*
	Destructor
*/
MaterialSystem::~MaterialSystem( )
{

}

/*
	Load material from file
*/
int MaterialSystem::LoadMaterial( const char* materialPath, const char* materialName )
{
	// Load material
	auto material = g_RenderSystem->LoadMaterial( materialPath, materialName );
	if( !material )
		return -1;

	material->mMaterialID = mMaterials.size( );
	mMaterials.push_back( material );
}

/*
	Create material from memory
*/
bool MaterialSystem::CreateMaterialFromMemory( const char* materialName, int width, int height, void* data, int offset )
{
	

	return true;
}

/*
	Get Material by index
*/
MaterialData_t* MaterialSystem::GetMaterial( int index )
{
	if( index < 0 || index > mMaterials.size( ) - 1 )
	{
		PrintError( "GetMaterial() -> Invalid index!" );
		return nullptr;
	}

	return mMaterials[ index ];
}

/*
	Get Material by name
*/
MaterialData_t* MaterialSystem::GetMaterial( const char* materialName, const char* materialPath )
{
	for( auto& mat : mMaterials )
	{
		if( materialName != nullptr )
		{
			if( CompareStrings( mat->mMaterialName, materialName ) )
			{
				return mat;
			}
		}
		else if( materialPath != nullptr )
		{
			if( CompareStrings( mat->mFile, materialPath ) )
			{
				return mat;
			}
		}
	}

	return nullptr;
}

/*
	Release material
*/
bool MaterialSystem::ReleaseMaterial( const char* materialName )
{
	int matIndex = 0;
	bool isExist = false;

	for( auto& mat : mMaterials )
	{
		if( CompareStrings( mat->mMaterialName, materialName ) )
		{
			g_RenderSystem->ReleaseMaterial( mat );

			isExist = true;
			break;
		}
		matIndex++;
	}

	// Delete from collection
	if( isExist )
	{
		mMaterials.erase( mMaterials.begin( ) + matIndex );
		return true;
	}

	return false;
}

/*
	Pre initialize module
*/
bool MaterialSystem::PreInit( )
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

	// Load Script System
	g_ScriptSystem = GetClassFromDll<IScriptSystem*>( "ScriptSystem.dll", "IScriptSystemV1" );
	if( !g_ScriptSystem )
	{
		PrintError( "Failed Get Script System!" );
		return false;
	}

	return true;
}

/*
	Initialize module
*/
bool MaterialSystem::Initialize( )
{

	return true;
}

/*
	Shutdown module
*/
void MaterialSystem::Shutdown( )
{
	// Release all textures
	for( auto& mat : mMaterials )
	{
		g_RenderSystem->ReleaseMaterial( mat );

		PrintMessage( "Texture (%s)(%s) was released!", mat->mFile, mat->mMaterialName );
	}

	mMaterials.clear( );
	mMaterials.shrink_to_fit( );
}

SaveDllClass( MaterialSystem, IMaterialSystemV1 )