/*

	Copyright 2024 Sergey Naumenkov

	File: StringTools.cpp
	Description: Tools for working with strings

	Date: 02/08/2024

*/

#include <string>

#include "StringTools.h"

/*
	Create new string and copy content from string to new string.
*/
char* CopyString( const char* in, long size )
{
	char* out = new char[ size + 1 ];

	/*for( long i = 0; i < size; i++ )
	{
		out[ i ] = in[ i ];
	}*/

	std::copy( in, in + size, out );
	out[ size ] = '\0';

	return out;
}

/*
	Compare two strings
*/
bool CompareStrings( const char* first, const char* second, int countToCompare )
{
	// Get string length
	auto fLength = StringLength( first );
	auto sLength = StringLength( second );

	if( countToCompare == 0 )
	{
		if( fLength != sLength )
		{
			return false;
		}
	}

	int count = countToCompare == 0 ? fLength : countToCompare;
	for( int i = 0; i < count; i++ )
	{
		if( first[ i ] != second[ i ] )
		{
			return false;
		}
	}

	return true;
}

/*
	Get string length
*/
int StringLength( const char* string )
{
	if( !string )
		return -1;

	auto str = string;
	int len = 0;

	while( string[ len ] != '\0' && string[ len ] != ' ' )
	{
		len++;
	}

	return len;
}

/*
	Fix slash in path
*/
const char* FixSlashesInString( const char* string )
{
	auto len = StringLength( string );
	char* str = CopyString( string, len );

	for( int i = 0; i < len; i++ )
	{
		if( str[ i ] == '\\' || str[ i ] == '/' )
		{
#ifdef _Windows
			str[ i ] = '\\';
#else
			str[ i ] = '/';
#endif
		}
	}

	return str;
}

/*
	Add first string to second string
*/
char* AddString( const char* first, const char* second )
{
	// Get length
	auto lenFirst = StringLength( first );
	auto lenSecond = StringLength( second );

	// Create out string
	auto total = lenFirst + lenSecond;
	char* out = new char[ total + 1 ];

	int cursorPos = 0;

	// Begin copy first string in out string
	for( int i = 0; i < lenFirst; i++ )
	{
		out[ cursorPos++ ] = first[ i ];
	}

	// After copy second string in out string
	for( int i = 0; i < lenSecond; i++ )
	{
		out[ cursorPos++ ] = second[ i ];
	}

	out[ cursorPos ] = '\0';

	return out;
}

/*
	Release string 
*/
void FreeString( const char*& string )
{
	delete string;
	string = nullptr;
}
