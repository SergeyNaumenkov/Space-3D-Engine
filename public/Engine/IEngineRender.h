/*

    Copyright 2024 Sergey Naumenkov

    File: IEngineRender.h
    Description: Public interface for render layer in engine

    Date: 07/08/2024

*/

#ifndef __Engine_IEngineRender_h__
#define __Engine_IEngineRender_h__
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

#include "Math/Vector3D.h"

class IEngineRender
{
public:

    // Initialize Engine Render System
    virtual bool OnInit( ) = 0;

    // Push View Matrix Data
    virtual void UpdateViewMatrixData( const Vector3Df& viewPosition, const Vector3Df& forwardVector, const Vector3Df& upVector ) = 0;

    // Lock system cursor by center of window
    virtual void LockSystemCursor( ) = 0;

    // Change show state for system cursor
    virtual void ShowSystemCursor( bool state ) = 0;
};

#endif // !__Engine_IEngineRender_h__
