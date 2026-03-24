/*

    Copyright 2024 Sergey Naumenkov

    File: EditorWindow.cpp
    Description: Main window for editor

    Date: 10/08/2024

*/

#include "EditorWindow.h"

#include "Core/Debug.h"
#include "InterfaceFactory/InterfaceFactory.h"

// ImGui

/*
    Window proc
*/
LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

/*
    Constructor
*/
EditorWindow::EditorWindow( )
{
    mWindow = nullptr;
    mWindowInstance = 0;
}

/*
    Destructor
*/
EditorWindow::~EditorWindow( )
{

}

/*
    Create editor window
*/
bool EditorWindow::CreateEditorWindow( int w, int h, int placeInCenter )
{
    WNDCLASS wnd = { };
    wnd.hInstance = mWindowInstance;
    wnd.lpfnWndProc = WindowProc;
    wnd.lpszClassName = "EditorWindowClassNameV1";

    // Register class
    RegisterClass( &wnd );

    mWindow = CreateWindowExA( 0, wnd.lpszClassName, "Space Level Editor",
                                       0, 0, 0,
                                       0, 0,
                                       0, 0,
                                       mWindowInstance, 0 );

    if( !mWindow )
    {
        PrintError( "Failed create editor window!" );
        return false;
    }

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    SetWindowLong( static_cast< HWND >( mWindow ), GWL_STYLE, style );

    if( placeInCenter )
    {
        // Set window position by center of screen
        int wScreen = GetSystemMetrics( SM_CXSCREEN );
        int hScreen = GetSystemMetrics( SM_CYSCREEN );

        // Calculate coords
        int xWindow = ( wScreen - w ) / 2;
        int yWindow = ( hScreen - h ) / 2;

        // Apply new styles and position
        SetWindowPos( static_cast< HWND >( mWindow ), 0, xWindow, yWindow, w, h,
                      SWP_SHOWWINDOW );
    }
    else
    {
        SetWindowPos( static_cast< HWND >( mWindow ), 0, 0, 0, w, h,
                      SWP_SHOWWINDOW );
    }

    return true;
}

/*
    Get editor window
*/
void* EditorWindow::GetWindow( )
{
    return mWindow;
}
/*
    Window proc
*/
LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
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