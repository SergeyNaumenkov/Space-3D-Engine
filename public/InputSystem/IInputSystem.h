/*

    Copyright 2024 Sergey Naumenkov

    File: IInputSystem.h
    Description: Public Interface for Input System

    Date: 05/08/2024

*/

#ifndef __InputSystem_InputSystem_h__
#define __InputSystem_InputSystem_h__
#pragma once

#include "Math/Vector2D.h"

#include "InputEvent.h"
#include "VirtualKeys.h"

#include "ModuleLoader\IModule.h"

class IInputSystem : public IModule
{
public:

    // Register window event
    virtual void RegisterMessage( unsigned int msg, long long firstParam, long long secondParam ) = 0;

    // Enable raw input
    virtual bool EnableRawInput( void* wnd ) = 0;

    // Process Buffered messages
    virtual void ProcessBufferedMessages( ) = 0;

    // Get Last event
    virtual InputDataEvent_t GetLastEvent( ) = 0;

    // Get mouse data
    virtual MouseEventData_t GetMouseData( ) = 0;

    // Reset mouse data
    virtual void ResetMouseData( ) = 0;

    // Is Keyboard key pressed
    virtual bool IsKeyDown( int keyCode ) = 0;

    // Get mouse position ( not raw )
    virtual Vector2D GetMousePosition( ) = 0;

    // Copy window ptr
    virtual void SetWindowPtr( void* wnd ) = 0;
};

#endif // !__InputSystem_InputSystem_h__
