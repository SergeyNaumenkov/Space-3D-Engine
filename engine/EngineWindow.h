/*

    Copyright 2024 Sergey Naumenkov

    File: EngineWindow.h
    Description: Managment class for Engine window's

    Date: 02/08/2024

*/

#ifndef __Engine_EngineWindow_h__
#define __Engine_EngineWindow_h__
#pragma once

#include "Platform/Windows.h"

/*
    Managements class for Engine window's
*/
class EngineWindow
{
public:
	EngineWindow( );
	~EngineWindow( );
    
    // Create engine window
    bool CreateEngineWindow( int w, int h, int placeInCenter );

    // Get window sizes
    void GetWindowSize( float& width, float& height );

    // Get window
    void* GetWindow( );

private:
    void* mWindow;
    HINSTANCE mWindowInstance;

    float mWindowWidth;
    float mWindowHeight;
};

#endif // !__Engine_EngineWindow_h__
