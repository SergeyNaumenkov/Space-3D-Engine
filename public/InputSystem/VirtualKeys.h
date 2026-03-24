/*

    Copyright 2024 Sergey Naumenkov

    File: VirtualKeys.h
    Description: Virtual keys for input system

    Date: 05/08/2024

*/

#ifndef __InputSystem_VirtualKeys_h__
#define __InputSystem_VirtualKeys_h__
#pragma once

enum VirtualKeys_t
{
    Key_A = 0x41,
    Key_B = 0x42,
    Key_C = 0x43,
    Key_D = 0x44,
    Key_E = 0x45,
    Key_F = 0x46,
    Key_G = 0x47,
    Key_H = 0x48,
    Key_I = 0x49,
    Key_J = 0x4A,
    Key_L = 0x4C,
    Key_M = 0x4D,
    Key_N = 0x4E,
    Key_O = 0x4F,
    Key_P = 0x50,
    Key_Q = 0x51,
    Key_R = 0x52,
    Key_S = 0x53,
    Key_T = 0x54,
    Key_U = 0x55,
    Key_V = 0x56,
    Key_W = 0x57,
    Key_X = 0x58,
    Key_Y = 0x59,
    Key_z = 0x5A,

    Key_0 = 0x30,
    Key_1 = 0x31,
    Key_2 = 0x32,
    Key_3 = 0x33,
    Key_4 = 0x34,
    Key_5 = 0x35,
    Key_6 = 0x36,
    Key_7 = 0x37,
    Key_8 = 0x38,
    Key_9 = 0x39,

    // Mouse buttons
    Key_MouseLeft = 0x01,
    Key_MouseRight = 0x02,
    Key_MouseMiddle = 0x04,

    // Modificators
    Key_Shift = 0x10,
    Key_LeftShift = 0xA1,
    Key_Control = 0x11,
    Key_LeftControl = 0xA3,
    Key_Tab = 0x09,
    Key_Escape = 0x1B,
    Key_Enter = 0x0D,
    Key_Space = 0x20,
    Key_Backspace = 0x08,
    Key_NumLock = 0x90,
    Key_LeftAlt = 0xA4,
    Key_RightAlt = 0xA5,

    // Arrows
    Key_ArrowLeft = 0x25,
    Key_ArrowRight = 0x27,
    Key_ArrowUp = 0x26,
    Key_ArrowDown = 0x28,

    // F keys
    Key_F1 = 0x70,
    Key_F2 = 0x71,
    Key_F3 = 0x72,
    Key_F4 = 0x73,
    Key_F5 = 0x74,
    Key_F6 = 0x75,
    Key_F7 = 0x76,
    Key_F8 = 0x77,
    Key_F9 = 0x78,
    Key_F10 = 0x79,
    Key_F11 = 0x7A,
    Key_F12 = 0x7B,
    Key_F13 = 0x7C,
    Key_F14 = 0x7D,
};

#endif // !__InputSystem_VirtualKeys_h__
