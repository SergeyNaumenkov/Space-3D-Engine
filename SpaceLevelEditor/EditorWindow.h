/*

    Copyright 2024 Sergey Naumenkov

    File: EditorWindow.h
    Description: Main window for editor

    Date: 10/08/2024

*/

#ifndef __SpaceLevelEditor_EditorWindow_h__
#define __SpaceLevelEditor_EditorWindow_h__
#pragma once

#include "Platform/Windows.h"

class EditorWindow
{
public:
	EditorWindow( );
	~EditorWindow( );

    // Create editor window
    bool CreateEditorWindow( int w, int h, int placeInCenter );

    // Get editor window
    void* GetWindow( );

private:
    void* mWindow;
    HINSTANCE mWindowInstance;
};

#endif // !__SpaceLevelEditor_EditorWindow_h__
