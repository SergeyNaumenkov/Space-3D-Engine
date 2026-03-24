/*

	Copyright 2024 Sergey Naumenkov

	File: Editor.cpp
	Description: Editor

	Date: 10/08/2024

*/

#include <cmath>
#include <DirectXCollision.h>
#include "Editor.h"

#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"
#include "ObjectTransform.h"
#include "MapCompiler.h"
#include "MapBox.h"
#include "SolidObjectsController.h"

#include "AmbientLight.h"

Editor::Editor( )
{
	mEditorWindow = nullptr;
	mRenderScene = nullptr;
}

Editor::~Editor( )
{
	delete mEditorWindow;
	delete mRenderScene;
}

SolidObjectsController controller;
AmbientLight* ambientLightTest;
int gAmbientBufferID = 0;

/*
	Initialize editor
*/
bool Editor::Initialize( )
{
	// Set base directory
	g_FileSystem->SetDirectory( "" );

	// Initialize editor window
	mEditorWindow = new EditorWindow( );
	if( !mEditorWindow->CreateEditorWindow( 1280, 720, true ) )
	{
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

	BaseRenderData_t render = { 1280, 720, mEditorWindow->GetWindow( ) };
	if( !g_RenderSystem->Initialize( render ) )
	{
		PrintError( "Failed initialize render system!" );
		return false;
	}

	// Initialize render scene
	mRenderScene = new RenderScene( );
	mRenderScene->Initialize( );

	// Enable raw input
	g_InputSystem->EnableRawInput( mEditorWindow->GetWindow( ) );

	// Initialize solid objects controller
	controller.Initialize( );

	IMapSolidObject* object = new MapSolidBox( );
	auto position = Vector3Df( 0, 0, 0 );
	auto size = Vector3Df( 32, 32, 32 );

	Vector3Df min = { position.x + -size.x,position.y + -size.y,position.z + -size.z };
	Vector3Df max = { position.x + size.x,position.y + size.y,position.z + size.z };
	BoundingBox_t bbox = { min, max };
	object->CreateFromBoundingBox( bbox );
	controller.AddSolidObject( object );

	object = new MapSolidBox( );
	position = Vector3Df( 0, 128, 0 );
	size = Vector3Df( 256, 32, 256 );

	min = { position.x + -size.x,position.y + -size.y,position.z + -size.z };
	max = { position.x + size.x,position.y + size.y,position.z + size.z };
	bbox = { min, max };
	object->CreateFromBoundingBox( bbox );
	controller.AddSolidObject( object );

	// Ambient light
	ambientLightTest = new AmbientLight( );
	ambientLightTest->CreateAmbientLight( { } );

	gAmbientBufferID = g_ShaderSystem->AllocateNewConstantBuffer( "LightConstantBuffer", sizeof( AmbientLightConstants_t ) );

	PrintMessage( "Editor initialized" );
	return true;
}

/*
	Start editor
*/
void Editor::Start( )
{
	PrintMessage( "Editor starting" );

	while( true )
	{
		if( MessagePump( ) )
		{
			break;
		}

		// Process input system data
		g_InputSystem->ProcessBufferedMessages( );

		g_RenderSystem->StartFrame( );

		// Update light
		auto amData = ambientLightTest->GetAmbientConstants( );
		AmbientLightConstants_t cbs = { };
		cbs.mAmbientColor = Vector4Df( 100, 100, 100, 255 );
		cbs.isEnabled = 1;

		g_ShaderSystem->UpdateConstantBufferData( ShaderType_Pixel, 0, "LightConstantBuffer", &cbs, sizeof( AmbientLightConstants_t ) );

		mRenderScene->GetCamera( )->Update( );
		controller.RenderSolidObjects( mRenderScene->GetCamera( ) );

		g_RenderSystem->EndFrame( );
	}
}

/*
	 Shutdown editor
*/
void Editor::Shutdown( )
{
	PrintMessage( "Editor shutdown" );
}

/*
	Process window messages
*/
bool Editor::MessagePump( )
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

/*
	 Render UI
*/
void Editor::RenderUserInterface( )
{

}
