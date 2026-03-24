/*

    Copyright 2024 Sergey Naumenkov

    File: AmbientLight.h
    Description: Ambient light

    Date: 20/08/2024

*/

#ifndef __SpaceLevelEditor_AmbientLight_h__
#define __SpaceLevelEditor_AmbientLight_h__
#pragma once

#include "ILightShared.h"

#include "Color.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

#include <DirectXMath.h>

/*
    Constant buffer for lights
*/
__declspec( align( 32 ) )
struct AmbientLightConstants_t
{
    Vector4Df mAmbientColor;
    int isEnabled;
};

/*
    Ambient light
*/
struct AmbientLight_t
{
    Vector4Df mAmbientColor;
    Vector3Df mAmbientLightDirection;
    int isEnabled;
};

class AmbientLight
{
public:
	AmbientLight( );
	~AmbientLight( );

    // Enable or disable ambient light
    void SetState( bool state );

    // Create ambient light
    void CreateAmbientLight( AmbientLight_t ambientLight );

    // Set ambient color
    void SetAmbientColor( Vector4Df newColor );

    // Get ambient data
    AmbientLight_t* GetAmbientLight( );

    // Get prepared Constants for shaders
    AmbientLightConstants_t GetAmbientConstants( );

private:
    AmbientLight_t* mCurrentAmbientLight;
};

#endif // !__SpaceLevelEditor_AmbientLight_h__
