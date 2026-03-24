/*

    Copyright 2024 Sergey Naumenkov

    File: Editor.h
    Description: Editor

    Date: 10/08/2024

*/

#ifndef __SpaceLevelEditor_Editor_h__
#define __SpaceLevelEditor_Editor_h__
#pragma once

#include "EditorWindow.h"
#include "RenderScene.h"

class Editor
{
public:
	Editor( );
	~Editor( );

    // Initialize editor
    bool Initialize( );

    // Start editor
    void Start( );

    // Shutdown editor
    void Shutdown( );

protected:

    // Process window messages
    bool MessagePump( );

    // Render UI
    void RenderUserInterface( );

private:
    EditorWindow* mEditorWindow;
    RenderScene* mRenderScene;
};

#endif // !__SpaceLevelEditor_Editor_h__
