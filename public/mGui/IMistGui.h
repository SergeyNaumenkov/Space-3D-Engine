/*

    Copyright 2024 Sergey Naumenkov

    File: IMistGui.h
    Description: Public interface for mGui library.

    Date: 10/09/2024

*/

#ifndef __mGui_IMistGui_h__
#define __mGui_IMistGui_h__
#pragma once

#include "Math/Vector2D.h"

class IMistGui
{
public:

    // Initialize library
    virtual bool Initialize( ) = 0;

    // Draw colored quad 
    virtual void DrawColorQuad( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadColor ) = 0;

    // Draw textured quad 
    virtual void DrawTexturedQuad( Vector2Df quadPosition, Vector2Df quadSize, unsigned long quadOverlayColor ) = 0;

    // Bind material
    virtual void BindMaterial( const char* materialName ) = 0;
    virtual void BindMaterial( int materialID ) = 0;

    // Draw all batched 2D geometry
    virtual void Render( ) = 0;

};

#endif // !__mGui_IMistGui_h__
