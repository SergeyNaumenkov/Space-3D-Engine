/*

    Copyright 2024 Sergey Naumenkov

    File: InterfaceFactory.h
    Description: Shared method for export class while during dll loads

    Date: 02/08/2024

*/

#ifndef __InterfaceFactory_InterfaceFactory_h__
#define __InterfaceFactory_InterfaceFactory_h__
#pragma once

#include <vector>

#include "Platform/Windows.h"

#include "Interfaces.h"

// Unload all modules by First In Last Out (FILO)
void UnloadAllModules( );

// Export symbol from dll
#define ExportFromDll __declspec(dllexport)

// Save exported class in dll
// 
#define SaveDllClass(_class,_functionName) \
extern "C" { ExportFromDll void* _functionName(){static _class cl;return &cl;}}

/// <summary>
/// Load or get dll handle and export class from dll.
/// All classes which be exported must be use 'SaveDllClass'
/// </summary>
/// <typeparam name="T:">Class</typeparam>
/// <param name="dll:">Name Of Dynamic Library</param>
/// <param name="classFuncName:">Specific name of export function. This name uses for searching function in exported dll</param>
/// <returns>Exported Class from loaded dll or nullptr</returns>
extern std::vector<HMODULE> g_loadedDlls;
template<class T> ExportFromDll T GetClassFromDll( const char* dll, const char* classFuncName )
{
	auto lib = GetModuleHandle( dll );
	if( !lib )
	{
		lib = LoadLibrary( dll );
		if( !lib )
		{
			auto rr = GetLastError( );
			return nullptr;
		}

		g_loadedDlls.push_back( lib );
	}

	if( lib )
	{
		auto proc = GetProcAddress( lib, classFuncName );
		if( proc != nullptr )
			return ( T )proc( );
		else
			return nullptr;
	}

	return nullptr;
}

#endif // !__InterfaceFactory_InterfaceFactory_h__
