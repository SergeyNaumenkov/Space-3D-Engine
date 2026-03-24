/*

    Copyright 2024 Sergey Naumenkov

    File: StringTools.h
    Description: Tools for working with strings

    Date: 02/08/2024

*/

#ifndef __Core_StringTools_h__
#define __Core_StringTools_h__
#pragma once

// Create new string and copy content from string to new string.
__declspec( dllexport ) char* CopyString( const char* in, long size );

// Compare two strings
__declspec( dllexport ) bool CompareStrings( const char* first, const char* second, int countToCompare = 0 );

// Get string length
__declspec( dllexport ) int StringLength( const char* string );

// Fix slash in path
__declspec( dllexport ) const char* FixSlashesInString( const char* string );

// Add first string to second string
__declspec( dllexport ) char* AddString( const char* first, const char* second );

// Release string 
__declspec( dllexport ) void FreeString( const char*& string );

#endif // !__Core_StringTools_h__
