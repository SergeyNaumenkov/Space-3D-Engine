/*

	Copyright 2024 Sergey Naumenkov

	File: MapCompiler.cpp
	Description: Map compiler for map ( output is binary file )

	Date: 10/08/2024

*/

#include "MapCompiler.h"

#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"

/*
	Constructor
*/
MapCompiler::MapCompiler( )
{
	mMapHeader = nullptr;
}

/*
	Destructor
*/
MapCompiler::~MapCompiler( )
{
	delete mMapHeader;
}

/*
	Let map compiler get info about all objects from scene
*/
void MapCompiler::SaveObjectsInfo( )
{
	
}

/*
	Write map file header into binary file
*/
void MapCompiler::WriteMapHeader( )
{
	mMapHeader = new MapFileHeader_t( );
	mMapHeader->mVersion = 'pams';
	mMapHeader->mMapCompilerVersion = 1222;

	// Write header
	g_FileSystem->WriteToFile( mMapHeader, sizeof( MapFileHeader_t ) );
}

/*
	Write cube data
*/
void MapCompiler::WriteCubeData(  )
{
	
}

