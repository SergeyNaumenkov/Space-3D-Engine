/*

    Copyright 2024 Sergey Naumenkov

    File: StringConverter.h
    Description: Convert string to another variable ( like a int or float )

    Date: 02/08/2024

*/

#ifndef __Core_StringConverter_h__
#define __Core_StringConverter_h__
#pragma once

class __declspec( dllexport )  StringConverter
{
public:
	StringConverter( const char* string );

    // Convert string to int
    int ToInt( );

    // Convert to float
    float ToFloat( );

    // Convert to long
    long ToLong( );

    // Don't convert, return usual string
    const char* GetString( );

private:
    const char* mString;
};

#endif // !__Core_StringConverter_h__
