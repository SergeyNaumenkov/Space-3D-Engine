/*

    Copyright 2024 Sergey Naumenkov

    File: MaterialLoader.h
    Description: Loader for materials

    Date: 14/09/2024

*/

#ifndef __RenderDx11_MaterialLoader_h__
#define __RenderDx11_MaterialLoader_h__
#pragma once

#include "MaterialSystem\MaterialData.h"
#include "DirectDrawSurface.h"
#include "StbLoader.h"

class MaterialLoader
{
public:
	MaterialLoader( );

    // Load material
    MaterialData_t* LoadMaterial( const char* materialPath, const char* materialName );

private:
    DirectDrawSurface* mDDSLoader;
    StbLoader* mStbLoader;
};

#endif // !__RenderDx11_MaterialLoader_h__
