/*

	Copyright 2024 Sergey Naumenkov

	File: EngineWindow.cpp
	Description: Managment for Engine window's

	Date: 02/08/2024

*/

#include "EngineWindow.h"

#include "InterfaceFactory/Interfaces.h"
#include "Core/Debug.h"

LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

/*
	Constructor
*/
EngineWindow::EngineWindow( )
{
	mWindow = nullptr;
	mWindowInstance = 0;
	mWindowWidth = mWindowHeight = 0;
}

/*
	Destructor
*/
EngineWindow::~EngineWindow( )
{

}

/*
	Create engine window
*/
bool EngineWindow::CreateEngineWindow( int w, int h, int placeInCenter )
{
	auto pScript = g_ScriptSystem->GetXmlScript( );

	if( !pScript->LoadScript( "resources\\Engine.xml", "EngineWindowScript" ) )
		return false;

	pScript->BindDocumentByName( "EngineWindowScript" );

	WNDCLASS wnd = { };
	wnd.hInstance = mWindowInstance;
	wnd.lpfnWndProc = WindowProc;
	wnd.lpszClassName = "EngineWindowClassNameV1";

	// Register class
	RegisterClass( &wnd );

	// Get title
	auto windowTitle = pScript->GetNodeValue( "EngineWindow", "WindowTitle" ).GetString( );

	// Create 
	mWindow = ( HWND )CreateWindowExA( 0, wnd.lpszClassName, windowTitle,
									   0, 0, 0,
									   0, 0,
									   0, 0,
									   mWindowInstance, 0 );

	if( !mWindow )
	{

		return false;
	}

	// apply styles
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	SetWindowLong( ( HWND )mWindow, GWL_STYLE, style );

	// Get window width and height
	auto windowWidth = pScript->GetNodeValue( "EngineWindow", "WindowWidth" ).ToInt( );
	auto windowHeight = pScript->GetNodeValue( "EngineWindow", "WindowHeight" ).ToInt( );

	// Place window in center of monitor
	// or in top left of monitor
	if( placeInCenter )
	{
		// Set window position by center of screen
		int wScreen = GetSystemMetrics( SM_CXSCREEN );
		int hScreen = GetSystemMetrics( SM_CYSCREEN );

		// Calculate coords
		int xWindow = ( wScreen - windowWidth ) / 2;
		int yWindow = ( hScreen - windowHeight ) / 2;

		// Apply new styles and position
		SetWindowPos( ( HWND )mWindow, 0, xWindow, yWindow, windowWidth, windowHeight,
					  SWP_SHOWWINDOW );
	}
	else
	{
		SetWindowPos( ( HWND )mWindow, 0, 0, 0, windowWidth, windowHeight,
					  SWP_SHOWWINDOW );
	}

	// Save window sizes
	mWindowWidth = static_cast< float >( windowWidth );
	mWindowHeight = static_cast< float >( windowHeight );

	return true;
}

/*
	 Get window sizes
*/
void EngineWindow::GetWindowSize( float& width, float& height )
{
	width = mWindowWidth;
	height = mWindowHeight;
}

/*
	Get window
*/
void* EngineWindow::GetWindow( )
{
	return mWindow;
}

/*
	Window proc
*/
LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// Send message to input system
	g_InputSystem->RegisterMessage( uMsg, wParam, lParam );
	switch( uMsg )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		default:
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
	}

	return true;
}