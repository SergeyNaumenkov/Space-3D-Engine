/*

    Copyright 2024 Sergey Naumenkov

    File: StringConverter.cpp
    Description: Convert string to another variable ( like a int or float )

    Date: 02/08/2024

*/

#include <iostream>

#include "StringConverter.h"

/*
    Constructor
*/
StringConverter::StringConverter( const char* string )
{
    mString = string;
}

/*
    Convert string to int
*/
int StringConverter::ToInt( )
{
    if( mString == nullptr )
        return -1;

    return atoi( mString );
}

/*
    Convert to float
*/
float StringConverter::ToFloat( )
{
    if( mString == nullptr )
        return -1;

    return static_cast< float >( atof( mString ) );
}

/*
    Convert to long
*/
long StringConverter::ToLong( )
{
    if( mString == nullptr )
        return -1;

    return atoi( mString );
}

/*
    Don't convert, return usual string
*/
const char* StringConverter::GetString( )
{
    return mString;
}
