/*

    Copyright 2024 Sergey Naumenkov

    File: DirectDrawSurface.h
    Description: Load Direct Draw Surface aka dds

    Date: 03/08/2024

*/

#ifndef __RenderDx11_DirectDrawSurface_h__
#define __RenderDx11_DirectDrawSurface_h__
#pragma once

#include "MaterialSystem\MaterialData.h"

class DirectDrawSurface
{
public:
	DirectDrawSurface( );

    // Load from file
    MaterialData_t* LoadFromFile( const char* file );

private:
};

#endif // !__RenderDx11_DirectDrawSurface_h__
