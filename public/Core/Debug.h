/*

    Copyright 2024 Sergey Naumenkov

    File: Debug.h
    Description: Shared functions for debugging

    Date: 02/08/2024

*/

#ifndef __Core_Debug_h__
#define __Core_Debug_h__
#pragma once

#include <iostream>

#include "Platform/Windows.h"

// Print usual message
__declspec( dllexport ) void PrintMessage( const char* format, ... );

// Print Warning message
__declspec( dllexport ) void PrintWarning( const char* format, ... );

// Print Error message
__declspec( dllexport ) void PrintError( const char* format, ... );

// Print Note message
__declspec( dllexport ) void PrintNote( const char* format, ... );

#endif // !__Core_Debug_h__
