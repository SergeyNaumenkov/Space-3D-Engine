/*

    Copyright 2024 Sergey Naumenkov

    File: MapCompiler.h
    Description: Map compiler for map ( output is binary file )

    Date: 10/08/2024

*/

#ifndef __SpaceLevelEditor_MapCompiler_h__
#define __SpaceLevelEditor_MapCompiler_h__
#pragma once

#include "Map/LevelMapDefs.h"

class MapCompiler
{
public:
	MapCompiler( );
	~MapCompiler( );

    // Let map compiler get info about all objects from scene
    void SaveObjectsInfo(  );

protected:

    // Write map file header into binary file
    void WriteMapHeader( );

    // Write cube data
    void WriteCubeData(  );

private:
    MapFileHeader_t* mMapHeader;
};

#endif // !__SpaceLevelEditor_MapCompiler_h__
