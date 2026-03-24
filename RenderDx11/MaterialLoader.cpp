/*

	Copyright 2024 Sergey Naumenkov

	File: MaterialLoader.cpp
	Description: Loader for materials

	Date: 14/09/2024

*/

#include "MaterialLoader.h"

// Interfaces:
#include "InterfaceFactory\InterfaceFactory.h"

// Core: 
#include "Core\StringTools.h"

/*
	Create new variable if 'var' is nullptr
*/
template<class T>
void AllocateClass( T*& var )
{
	if( !var )
	{
		var = new T( );
	}
}

/*
	Constructor
*/
MaterialLoader::MaterialLoader( )
{
	mDDSLoader = nullptr;
	mStbLoader = nullptr;
}

/*
	Load material
*/
MaterialData_t* MaterialLoader::LoadMaterial( const char* materialPath, const char* materialName )
{
	if( !g_FileSystem->IsFileExist( materialPath ) )
		return nullptr;

	MaterialData_t* materialData = nullptr;

	// Get file extension
	auto fileExtension = g_FileSystem->GetFileExtension( materialPath );
	if( CompareStrings( fileExtension, "dds" ) )
	{
		AllocateClass<DirectDrawSurface>( mDDSLoader );

		// Load dds material
		materialData = mDDSLoader->LoadFromFile( materialPath );
	}
	else
	{
		// Load another image files
		AllocateClass<StbLoader>( mStbLoader );

		// Load material
		materialData = mStbLoader->LoadMaterial( materialPath );
	}

	// if material is loaded - return it
	if( materialData )
	{
		materialData->mMaterialName = materialName;
		return materialData;
	}

	return nullptr;
}


