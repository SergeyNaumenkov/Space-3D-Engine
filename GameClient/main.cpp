/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: Game Client

	Date: 04/08/2024

*/

#include "IGameClient.h"

#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"
#include "Player.h"

class GameClient : public IGameClient
{
public:
	GameClient( );
	~GameClient( );

	// Initialize game client
	virtual bool OnInit( );

	// Start game client 
	virtual void StartGame( );

	// Shutdown game client 
	virtual void Shutdown( );

private:
	Player* mPlayer;

	bool mLockSystemCursor;
};

/*
	Constructor
*/
GameClient::GameClient( )
{
	mPlayer = nullptr;
	mLockSystemCursor = true;
}

/*
	Destructor
*/
GameClient::~GameClient( )
{
	delete mPlayer;
}

/*
	Initialize game client
*/
bool GameClient::OnInit( )
{
	// Load engine
	g_EngineSystem = GetClassFromDll<IEngineSystem*>( "Engine.dll", "EngineSystemMainV1" );
	if( !g_EngineSystem )
	{
		return false;
	}

	// Load Input System
	g_InputSystem = GetClassFromDll<IInputSystem*>( "InputSystem.dll", "IInputSystemV1" );
	if( !g_InputSystem )
	{
		PrintError( "Failed Get Input System!" );
		return false;
	}

	// Enable raw input
	g_InputSystem->EnableRawInput( g_EngineSystem->GetEngineWindow( ) );

	// Load render system
	auto dll = LoadLibrary( "BaseRenderer.dll" );
	if( !dll )
		return false;

	typedef IBaseRenderer* ( *GetBaseRendererFunc )( const char* renderName );
	GetBaseRendererFunc func = ( GetBaseRendererFunc )GetProcAddress( dll, "GetBaseRenderer" );
	g_RenderSystem = func( "DirectX11" );
	FreeLibrary( dll );

	// Test of update view matrix in render system
	mPlayer = new Player( );
	mPlayer->OnInit( );
	mPlayer->Spawn( Vector3Df( 0, 40, -32 ), Vector3Df( 0, 0, 0 ) );

	return true;
}

/*
	Start game client
*/
void GameClient::StartGame( )
{
	// Lock system cursor
	if( g_InputSystem->IsKeyDown( Key_F1 ) )
	{
		mLockSystemCursor = !mLockSystemCursor;
	}

	mPlayer->Update( );
}

/*
	Shutdown game client
*/
void GameClient::Shutdown( )
{

}

SaveDllClass( GameClient, IGameClientV1 )