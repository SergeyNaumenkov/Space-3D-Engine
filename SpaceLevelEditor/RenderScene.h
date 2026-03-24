/*

    Copyright 2024 Sergey Naumenkov

    File: RenderScene.h
    Description: 3D Render Scene

    Date: 10/08/2024

*/

#ifndef __SpaceLevelEditor_RenderScene_h__
#define __SpaceLevelEditor_RenderScene_h__
#pragma once

#include <d3d11_1.h>

#include "3DCamera.h"

class RenderScene
{
public:
	RenderScene( );
	~RenderScene( );

    // Initialize render scene
    bool Initialize( );

    // Render scene
    void Render( );

    // Add Cube to scene
    void AddCube( Vector3Df position, Vector3Df size );

    // Try ray cast object, if mouse was pressed
    // Detected object will be saved
    void TryRayCastObject( HWND wnd );

    // Get camera
    Render3DCamera* GetCamera( );

protected:
   
    

private:
    Render3DCamera* mRenderCamera;
    int mRayCastSavedCubeIndex;
};

#endif // !__SpaceLevelEditor_RenderScene_h__
