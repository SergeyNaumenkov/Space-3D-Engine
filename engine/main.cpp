/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: Main file for engine

	Date: 02/08/2024

*/

#include "EngineTime.h"
#include "EngineWindow.h"
#include "IEngineSystem.h"

#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"

class EngineSystem : public IEngineSystem
{
public:
	EngineSystem( );
	~EngineSystem( );

	// Pre initialize module
	virtual bool PreInit( ) override;

	// Initialize module
	virtual bool Initialize( ) override;

	// Start 
	virtual void Start( ) override;

	// Shutdown module
	virtual void Shutdown( ) override;

	// Get Engine Window 
	virtual void* GetEngineWindow( );

	// Get Engine window sizes
	virtual void GetEngineWindowSize( float& width, float& height );

	// Get screen sizes
	virtual void GetScreenSizes( float& width, float& height );

	// Get elapsed time
	virtual float GetElapsedTime( );

protected:

	// Processing of Window Messages
	bool MessagePump( );

private:
	EngineWindow* mEngineWindow;
	EngineTime* mEngineTimer;

	float mFrameTime;
};

/*
	Constructor
*/
EngineSystem::EngineSystem( )
{
	mEngineWindow = nullptr;
	mEngineTimer = nullptr;
	mFrameTime = 0.0f;
}

/*
	Destructor
*/
EngineSystem::~EngineSystem( )
{
	delete mEngineWindow;
}

/*
	Initialize stuff which neeed for OnInit
*/
bool EngineSystem::PreInit( )
{
	// File System
	g_FileSystem = GetClassFromDll<IFileSystem*>( "FileSystem.dll", "IFileSystemV1" );
	if( !g_FileSystem )
	{
		PrintError( "Failed Get File System!" );
		return false;
	}

	// Load Input System
	g_InputSystem = GetClassFromDll<IInputSystem*>( "InputSystem.dll", "IInputSystemV1" );
	if( !g_InputSystem )
	{
		PrintError( "Failed Get Input System!" );
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

	// Load Material System
	g_MaterialSystem = GetClassFromDll<IMaterialSystem*>( "MaterialSystem.dll", "IMaterialSystemV1" );
	if( !g_MaterialSystem )
	{
		PrintError( "Failed Get Material System!" );
		return false;
	}

	// Load Script System
	g_ScriptSystem = GetClassFromDll<IScriptSystem*>( "ScriptSystem.dll", "IScriptSystemV1" );
	if( !g_ScriptSystem )
	{
		PrintError( "Failed Get Script System!" );
		return false;
	}

	// Load game client 
	PrintMessage( "Loading game client dll!" );
	g_GameClient = GetClassFromDll<IGameClient*>( "GameClient.dll", "IGameClientV1" );
	if( !g_GameClient )
	{
		PrintError( "Failed Get Game client!" );
		return false;
	}

	return true;
}

/*
	Initialize module
*/
bool EngineSystem::Initialize( )
{
	// Engine Window
	mEngineWindow = new EngineWindow( );

	// Set new dir for engine and all submodules
	g_FileSystem->SetDirectory( "" );

	if( !mEngineWindow->CreateEngineWindow( 1280, 720, true ) )
	{
		PrintError( "Failed Create Engine Window!" );
		return false;
	}

	// Initialize render system
	BaseRenderData_t renderData = { };
	float w = 0, h = 0;
	mEngineWindow->GetWindowSize( w, h );
	renderData.mWindowWidth = w;
	renderData.mWindowHeight = h;
	renderData.mWindowPtr = mEngineWindow->GetWindow( );

	if( !g_RenderSystem->Initialize( renderData ) )
	{
		PrintError( "Failed initialize render system!" );
		return false;
	}

	// Initialize Shader System
	/*if( !g_ShaderSystem->OnInit( ) )
	{
		PrintError( "Failed Initialize Shader System!" );
		return false;
	}*/

	// Initialize Game Client
	if( !g_GameClient->OnInit( ) )
	{
		PrintError( "Failed initialize Game Client" );
		return false;
	}

	// Initialize engine timer
	mEngineTimer = new EngineTime( );
	if( !mEngineTimer->OnInit( ) )
	{
		return false;
	}

	return true;
}

bool filter( float t )
{
	float fps = 300.f;
	if( fps > 0.0f )
	{
		// Limit fps to withing tolerable range
		fps = min( 1000.f, fps );

		float minframetime = 1.0f / fps;

		if( t < minframetime )
		{
			// framerate is too high
			return false;
		}
	}

	return true;
}

/*
	Start engine
*/
void EngineSystem::Start( )
{
	while( true )
	{
		if( MessagePump( ) )
			break;

		// For debug, because right now we don't have a game console
		if( g_InputSystem->IsKeyDown( Key_Escape ) )
			break;

		mEngineTimer->Start( );
		mFrameTime += mEngineTimer->GetElapsedTime2( );

		if( !filter( mFrameTime ) )
			continue;

		mEngineTimer->SetFrameTime( mFrameTime );
		g_InputSystem->ProcessBufferedMessages( );
		g_RenderSystem->StartFrame( );

		// Game loop
		g_GameClient->StartGame( );
		g_RenderSystem->EndFrame( );

		mFrameTime = 0;
	}
}

/*
	Shutdown engine
*/
void EngineSystem::Shutdown( )
{
	PrintMessage( "Engine is shutdown" );

	g_RenderSystem->Shutdown( );
}

/*
	Get Engine Window
*/
void* EngineSystem::GetEngineWindow( )
{
	return mEngineWindow->GetWindow( );
}

/*
	Get Engine window sizes
*/
void EngineSystem::GetEngineWindowSize( float& width, float& height )
{
	mEngineWindow->GetWindowSize( width, height );
}

/*
	Get screen sizes
*/
void EngineSystem::GetScreenSizes( float& width, float& height )
{
	// Right now getting the sizes here
	// But later must be get from render system
	width = static_cast< float >( GetSystemMetrics( SM_CXSCREEN ) );
	height = static_cast< float >( GetSystemMetrics( SM_CYSCREEN ) );
}

/*
	Get elapsed time
*/
float EngineSystem::GetElapsedTime( )
{
	return mEngineTimer->GetElapsedTime( );
}

/*
	Processing of Window Messages
*/
bool EngineSystem::MessagePump( )
{
	MSG msg;
	while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );

		if( msg.message == WM_QUIT )
			return true;
	}

	return false;
}

SaveDllClass( EngineSystem, EngineSystemMainV1 )