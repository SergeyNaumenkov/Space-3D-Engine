/*

    Copyright 2024 Sergey Naumenkov

    File: InterfaceFactory.cpp
    Description: Shared method for export class while during dll loads

    Date: 02/08/2024

*/

#include <windows.h>

#include "InterfaceFactory.h"

std::vector<HMODULE> g_loadedDlls;

/*
	Unload all modules by First In Last Out (FILO)
*/
void UnloadAllModules( )
{
	while( g_loadedDlls.size( ) > 0 )
	{
		auto handle = g_loadedDlls.back( );
		FreeLibrary( handle );
		g_loadedDlls.pop_back( );
	}
}
