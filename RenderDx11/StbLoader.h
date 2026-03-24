/*

    Copyright 2024 Sergey Naumenkov

    File: StbLoader.h
    Description: Loads textures like a png jpg and etc with STB Image Loader

    Date: 14/09/2024

*/

#ifndef __RenderDx11_StbLoader_h__
#define __RenderDx11_StbLoader_h__
#pragma once

#include "MaterialSystem\MaterialData.h"
#include "RenderDevicesInternal.h"

class StbLoader
{
public:
	StbLoader( );

    // Load material 
    MaterialData_t* LoadMaterial( const char* materialPath );

protected:

    // Create texture
    ID3D11ShaderResourceView* CreateTexture(unsigned char* imageBuffer, int width, int height, int channelCount );

private:

};
#endif // !__RenderDx11_StbLoader_h__
