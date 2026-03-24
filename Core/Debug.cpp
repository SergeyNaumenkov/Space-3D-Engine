/*

	Copyright 2024 Sergey Naumenkov

	File: Debug.cpp
	Description: Shared functions for debugging

	Date: 02/08/2024

*/

#include "Debug.h"

// max buffer size
const int MessageBufferSize = 1024;

/*
	Core for all print functions
*/
void PrintCore( const char* messageType, const char* format, va_list lst )
{
	char buffer[ MessageBufferSize ] = { '\0' };

	vsnprintf_s( buffer, sizeof( buffer ), format, lst );

	char time[ 10 ] = { '\0' };
	_strtime_s( time );

	std::string out = "";
	out += time;
	out += " [";
	out += messageType;
	out += "] ";
	out += buffer;
	out += "\n";

//#ifdef _AllowMessageToConsole
	printf( "%s", out.c_str( ) );
//#endif

	//OutputDebugString( out.c_str( ) );
}

/*
	Print usual message
*/
void PrintMessage( const char* format, ... )
{
	va_list lst;
	va_start( lst, format );
	PrintCore( "Message", format, lst );
	va_end( lst );
}

/*
	Print Warning message
*/
void PrintWarning( const char* format, ... )
{
	va_list lst;
	va_start( lst, format );
	PrintCore( "Warning", format, lst );
	va_end( lst );
}

/*
	Print Error message
*/
void PrintError( const char* format, ... )
{
	va_list lst;
	va_start( lst, format );
	PrintCore( "Error", format, lst );
	va_end( lst );
}

/*
	Print Note message
*/
void PrintNote( const char* format, ... )
{
	va_list lst;
	va_start( lst, format );
	PrintCore( "Note", format, lst );
	va_end( lst );
}