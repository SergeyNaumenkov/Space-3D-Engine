/*

	Copyright 2024 Sergey Naumenkov

	File: DirectDrawSurface.cpp
	Description: Load Direct Draw Surface aka dds

	Date: 03/08/2024

*/

#include "DirectDrawSurface.h"

#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"

#include "RenderDevicesInternal.h"

// From public
#include "DDSTextureLoader.h"

DirectDrawSurface::DirectDrawSurface( )
{

}

MaterialData_t* DirectDrawSurface::LoadFromFile( const char* file )
{
	// Check exist file
	if( !g_FileSystem->IsFileExist( file ) )
	{
		PrintError( "Texture (%s) is not exist!", file );
		return { };
	}

	// Get render device
	auto device = GetRenderDevicesInternal( )->mDevice;
	if( !device )
	{
		PrintError( "Failed load (%s) because method can't get the Render Device!", file );
		return { };
	}

	// Open and Read texture
	long fileSize = 0;
	g_FileSystem->SetOpenMode( FileMode_t::File_OpenBinary );
	auto ddsBuffer = g_FileSystem->OpenAndReadFile( file, &fileSize );

	// Create texture
	ID3D11ShaderResourceView* pTexture = nullptr;
	auto result = DirectX::CreateDDSTextureFromMemory( device,
										 reinterpret_cast< const uint8_t* >( ddsBuffer ),
										 fileSize, nullptr, &pTexture );

	if( FAILED( result ) )
	{
		PrintError( "Failed compile texture (%s)!", file );
		return { };
	}

	// Get texture information
	ID3D11Texture2D* pTextureInterface = 0;
	ID3D11Resource* res;

	pTexture->GetResource( &res );
	res->QueryInterface<ID3D11Texture2D>( &pTextureInterface );

	D3D11_TEXTURE2D_DESC desc;
	pTextureInterface->GetDesc( &desc );

	// Save out compiled texture
	MaterialData_t* dds = new MaterialData_t( );
	dds->mFile = file;
	dds->mFormat = desc.Format;
	dds->mWidth = desc.Width;
	dds->mHeight = desc.Height;
	dds->MipLevel = desc.MipLevels;
	dds->mTexture = pTexture;

	pTextureInterface->Release( );
	pTextureInterface = nullptr;
	
	res->Release( );
	res = nullptr;

	return dds;
}
