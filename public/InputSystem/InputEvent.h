/*

    Copyright 2024 Sergey Naumenkov

    File: InputEvent.h
    Description: Input events

    Date: 05/08/2024

*/

#ifndef __InputSystem_InputEvent_h__
#define __InputSystem_InputEvent_h__
#pragma once

enum InputEventType_t
{
    InputEvent_MouseLeftButtonDown,
    InputEvent_MouseLeftButtonUp,

    InputEvent_MouseRightButtonDown,
    InputEvent_MouseRightButtonUp,

    InputEvent_MouseMove,

    InputEvent_KeyDown,
    InputEvent_KeyUp,
};

struct MouseEventData_t
{
    int mX;
    int mY;
    long long mAdditionalKey;
};

struct KeyboardEventData_t
{
    long long mKey;
    long long mSecondKey;
};

struct InputDataEvent_t
{
    InputEventType_t mEventType;
    MouseEventData_t mMouseData;
    KeyboardEventData_t mKeyboardData;
};

#endif // !__InputSystem_InputEvent_h__
