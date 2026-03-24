/*

    Copyright 2024 Sergey Naumenkov

    File: main.cpp
    Description: Test for GUI system

    Date: 29/08/2024

*/

#include "InterfaceFactory\InterfaceFactory.h"
#include "Core/Debug.h"
#include "Color.h"

/*
	Window proc
*/
LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_KILLFOCUS:
			break;

		case WM_LBUTTONDOWN:
			break;

		case WM_LBUTTONUP:
			break;

		case WM_CHAR:
			break;

		case WM_MOUSEMOVE:
			POINTS s = MAKEPOINTS( lParam );
			/*mouseX = s.x;
			mouseY = s.y;*/
			break;

		/*case WM_SETFOCUS:
			break;*/

		/*default:
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
			break;*/
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

/*
	Create test window
*/
HWND CreateTestWindow( int w, int h )
{
	WNDCLASS wnd = { };
	wnd.hInstance = GetModuleHandle( 0 );
	wnd.lpszClassName = "RendererWindowClassV1";
	wnd.lpfnWndProc = WindowProc;

	// register our class
	RegisterClass( &wnd );

	// Create window
	HWND m_pWindow = CreateWindowEx(
		0,
		wnd.lpszClassName,
		"Renderer test window",
		0,
		0, 0,
		w, h,
		0,
		0,
		GetModuleHandle( 0 ),
		0
	);

	if( !m_pWindow )
	{
		return 0;
	}

	// Display window 
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	SetWindowLong( ( HWND )m_pWindow, GWL_STYLE, style );

	// Set window position by center of screen
	int wScreen = GetSystemMetrics( SM_CXSCREEN );
	int hScreen = GetSystemMetrics( SM_CYSCREEN );

	// Calculate coords
	int xWindow = ( wScreen - w ) / 2;
	int yWindow = ( hScreen - h ) / 2;

	// Apply new styles and position
	SetWindowPos( ( HWND )m_pWindow, 0, xWindow, yWindow, w, h,
				  SWP_SHOWWINDOW );

	return m_pWindow;
}

// Load renderer
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
	g_ShaderSystem->PreInit( );
	g_ShaderSystem->Initialize( );

	// Load Material System
	g_MaterialSystem = GetClassFromDll<IMaterialSystem*>( "MaterialSystem.dll", "IMaterialSystemV1" );
	if( !g_MaterialSystem )
	{
		PrintError( "Failed Get Material System!" );
		return false;
	}
	g_MaterialSystem->PreInit( );
	g_MaterialSystem->Initialize( );

	// Load Input System
	g_InputSystem = GetClassFromDll<IInputSystem*>( "InputSystem.dll", "IInputSystemV1" );
	if( !g_InputSystem )
	{
		PrintError( "Failed load Input System!" );
		return false;
	}

	// Load gui here!
	g_mGui = GetClassFromDll<IMistGui*>( "mGui.dll", "IMistGuiV1" );
	if( !g_mGui )
	{
		PrintError( "Failed load Mist Gui!" );
		return false;
	}

	return true;
}

bool InitApp( )
{
	const int w = 1280;
	const int h = 720;
	auto wnd = CreateTestWindow( w, h );
	if( !wnd )
	{
		return 1;
	}

	BaseRenderData_t rData = { };
	rData.mWindowPtr = wnd;
	rData.mWindowWidth = w;
	rData.mWindowHeight = h;

	if( !g_RenderSystem->Initialize( rData ) )
		return false;

	// Load gui
	g_mGui->Initialize( );

	g_InputSystem->SetWindowPtr( wnd );
}

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

int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	CreateConsoleWindow( );

	LoadModules( );
	g_FileSystem->SetDirectory( "" );

	if( !InitApp( ) )
		return 1;

	g_MaterialSystem->LoadMaterial( "materials\\button.png", "DefaultButton" );

	bool bWantQuit = false;
	while( true )
	{
		MSG msg;
		while( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );

			if( msg.message == WM_QUIT )
			{
				bWantQuit = true;
				break;
			}
		}

		if( bWantQuit )
			break;

		g_RenderSystem->StartFrame( );
		g_RenderSystem->SetDepthState( false );
		
		g_mGui->BindMaterial( "DefaultButton" );

		g_mGui->DrawTexturedQuad( Vector2Df( 12, 12 ), Vector2Df( 23, 23 ), Color( 255, 255, 255, 255 ).ToLong( ) );
		g_mGui->DrawColorQuad( Vector2Df( 12, 60 ), Vector2Df( 120, 34 ), Color( 255, 255, 255, 255 ).ToLong( ) );

		g_mGui->Render( );

		g_RenderSystem->EndFrame( );
	}

	return 0;
}
