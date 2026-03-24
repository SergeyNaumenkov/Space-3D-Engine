/*

    Copyright 2024 Sergey Naumenkov

    File: Display.h
    Description: Stores information about all display modes

    Date: 06/09/2024

*/

#ifndef __RenderDx11_Display_h__
#define __RenderDx11_Display_h__
#pragma once

#include <d3d11_1.h>

#include "Core/Vector.h"

struct DisplayMode_t
{
    unsigned int mWidth;
    unsigned int mHeight;

    unsigned int mRefreshRate;

};

class Display
{
public:
	Display( );

    // Initialize and get display modes
    bool Initialize( IDXGIFactory2*& dxgiFactory );

    // Get mode
    const DisplayMode_t& GetDisplayMode( int index );

    // Get total count of modes
    const int GetModesCount( );

    // Shutdown
    void Shutdown( );

protected:

    // Parse modes
    void ParseModes( );

private:
    DXGI_MODE_DESC* mDxgiModes;
    unsigned int mDxgiModesCount;

    Vector<DisplayMode_t> mModes;
    int mModesCount;
};

#endif // !__RenderDx11_Display_h__
