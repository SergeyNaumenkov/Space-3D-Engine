/*

    Copyright 2024 Sergey Naumenkov

    File: FontManager.h
    Description: Font manager

    Date: 08/09/2024

*/

#ifndef __FontManager_FontManager_h__
#define __FontManager_FontManager_h__
#pragma once

#include "FontData.h"
#include "Core/Vector.h"

class FontManager
{
public:
	FontManager( );
	~FontManager( );

    // Load font from file
    bool LoadFont( const char* filePath, const char* fontName, int fontSize );

private:
};
#endif // !__FontManager_FontManager_h__
