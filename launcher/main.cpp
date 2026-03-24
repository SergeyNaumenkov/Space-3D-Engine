/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: Launcher for engine.

	Date: 02/08/2024

*/

#include "Platform/Windows.h"

#include "Engine/IEngineSystem.h"
#include "InterfaceFactory/InterfaceFactory.h"
#include "ModuleLoader\ModuleLoader.h"

/*
	Create Debug Console
*/
void CreateConsoleWindow( )
{
	// alloc new console
	AllocConsole( );

	// Redirect default flows to our allocated console
	FILE* fp;
	freopen_s( &fp, "CONOUT$", "w", stdout );
	freopen_s( &fp, "CONOUT$", "w", stderr );
	freopen_s( &fp, "CONIN$", "r", stdin );

	// place on top-left corner of screen
	HWND consoleWindow = GetConsoleWindow( );
	SetWindowPos( consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
}

std::vector<ModuleData_t> g_Modules = {
	{"FileSystem.dll", "IFileSystemV1" },
	{"InputSystem.dll", "IInputSystemV1" },
	{"MaterialSystem.dll", "IMaterialSystemV1" },
	{"ShaderSystem.dll", "IShaderSystemV1" },
	{"Engine.dll", "EngineSystemMainV1" },
};

/*
	Entry point for launcher
*/
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
#ifdef _DebugBuild
	CreateConsoleWindow( );
#endif

	ModuleLoader modules;

	auto isDone = modules.LoadModules( g_Modules );
	if( !isDone )
		return 1;

	isDone = modules.PreInit( );
	if( !isDone )
		return 1;

	isDone = modules.Initialize( );
	if( !isDone )
		return 1;

	modules.Start( );

	modules.UnloadModules( );

	return 0;
}