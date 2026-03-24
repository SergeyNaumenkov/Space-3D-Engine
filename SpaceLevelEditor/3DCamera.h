/*

    Copyright 2024 Sergey Naumenkov

    File: 3DCamera.h
    Description: 3D camera for editor

    Date: 10/08/2024

*/

#ifndef __SpaceLevelEditor_3DCamera_h__
#define __SpaceLevelEditor_3DCamera_h__
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

#include "Math/Vector3D.h"

class Render3DCamera
{
public:
	Render3DCamera( );
	~Render3DCamera( );

    // Initialize camera
    void Initialize( );

    // Update camera
    void Update( );

    // Get camera position
    Vector3Df GetPosition( );

    // Get camera view angle
    Vector3Df GetViewAngle( );

    // Enable or disable camera input 
    void SetCameraInputState( bool state );

    // Get view matrix
    XMMATRIX GetViewMatrix( );
    
    // Get projection matrix
    XMMATRIX GetProjectionMatrix( );

protected:

    // Process mouse input 
    void ProcessMouseInput( );

    // Process keyboard input 
    void ProcessKeyboardInput( );

    // Update matrix
    void UpdateMatrix( );

private:
    XMMATRIX mViewMatrix;
    XMMATRIX mProjectionMatrix;

    Vector3Df mCameraPosition;
    Vector3Df mPlayerVelocity;
    Vector3Df mCameraViewAngle;

    bool mAllowFreeMove;
};

#endif // !__SpaceLevelEditor_3DCamera_h__
