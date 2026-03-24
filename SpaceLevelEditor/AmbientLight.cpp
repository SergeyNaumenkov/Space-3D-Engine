/*

    Copyright 2024 Sergey Naumenkov

    File: AmbientLight.cpp
    Description: Ambient light

    Date: 20/08/2024

*/

#include "AmbientLight.h"

/*
    Constructor
*/
AmbientLight::AmbientLight( )
{
    mCurrentAmbientLight = nullptr;
}

/*
    Destructor
*/
AmbientLight::~AmbientLight( )
{
    delete mCurrentAmbientLight;
}

/*
    Enable or disable ambient light
*/
void AmbientLight::SetState( bool state )
{
    mCurrentAmbientLight->isEnabled = state;
}

/*
    Create ambient light
*/
void AmbientLight::CreateAmbientLight( AmbientLight_t ambientLight )
{
    mCurrentAmbientLight = new AmbientLight_t( ambientLight );
}

/*
    Set ambient color
*/
void AmbientLight::SetAmbientColor( Vector4Df newColor )
{
    mCurrentAmbientLight->mAmbientColor = newColor;
}

/*
    Get ambient data
*/
AmbientLight_t* AmbientLight::GetAmbientLight( )
{
    return mCurrentAmbientLight;
}

/*
    Get prepared Constants for shaders
*/
AmbientLightConstants_t AmbientLight::GetAmbientConstants( )
{
    return AmbientLightConstants_t( mCurrentAmbientLight->mAmbientColor, mCurrentAmbientLight->isEnabled );
}

