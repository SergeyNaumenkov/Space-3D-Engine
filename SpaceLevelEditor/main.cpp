/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: Entry point for Game Level Editor
	Note:This is test level editor.
	It have own render system. Later I connect level editor with our Render System.

	Date: 10/08/2024

*/

#include <iostream>

#include "ModuleLoader\ModuleLoader.h"
#include "Editor.h"

#include "Platform/Windows.h"
#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"

#include "Map/LevelMapDefs.h"

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

// From launcher
std::vector<ModuleData_t> g_Modules = {
	{"FileSystem.dll", "IFileSystemV1" },
	{"InputSystem.dll", "IInputSystemV1" },
	{"MaterialSystem.dll", "IMaterialSystemV1" },
	{"ShaderSystem.dll", "IShaderSystemV1" },
};

bool LoadModules( )
{
	// Load File System
	g_FileSystem = GetClassFromDll<IFileSystem*>( "FileSystem.dll", "IFileSystemV1" );
	if( !g_FileSystem )
	{
		PrintError( "Failed load file system!" );
		return false;
	}

	// Load render system
	auto dll = LoadLibrary( "BaseRenderer.dll" );
	if( !dll )
		return false;

	typedef IBaseRenderer* ( *GetBaseRendererFunc )( const char* renderName );
	GetBaseRendererFunc func = ( GetBaseRendererFunc )GetProcAddress( dll, "GetBaseRenderer" );
	g_RenderSystem = func( "DirectX11" );
	FreeLibrary( dll );

	// Load Shader System
	g_ShaderSystem = GetClassFromDll<IShaderSystem*>( "ShaderSystem.dll", "IShaderSystemV1" );
	if( !g_ShaderSystem )
	{
		PrintError( "Failed load shader system!" );
		return false;
	}

	// Load Material System
	g_MaterialSystem = GetClassFromDll<IMaterialSystem*>( "MaterialSystem.dll", "IMaterialSystemV1" );
	if( !g_MaterialSystem )
	{
		PrintError( "Failed Get Material System!" );
		return false;
	}

	g_MaterialSystem->PreInit( );

	// Load Input System
	g_InputSystem = GetClassFromDll<IInputSystem*>( "InputSystem.dll", "IInputSystemV1" );
	if( !g_InputSystem )
	{
		PrintError( "Failed load Input System!" );
		return false;
	}

	return true;
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
#ifdef _DebugBuild
	CreateConsoleWindow( );
#endif
	PrintMessage( "Level editor is starting!" );

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

	// load modules
	if( !LoadModules( ) )
		return 1;

	PrintMessage( "All modules was loaded!" );

	Editor* editor = new Editor( );
	if( !editor->Initialize( ) )
	{
		PrintError( "Failed initialize editor!" );
		return 1;
	}

	modules.Start( );

	// Start editor
	editor->Start( );

	modules.UnloadModules( );

	// Shutdown editor
	editor->Shutdown( );

	return 0;
}