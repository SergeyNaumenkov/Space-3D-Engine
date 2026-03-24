/*

    Copyright 2024 Sergey Naumenkov

    File: IEngineSystem.h
    Description: Public Interface For all sub modules

    Date: 02/08/2024

*/

#ifndef __Engine_IEngineSystem_h__
#define __Engine_IEngineSystem_h__
#pragma once

#include "IEngineRender.h"
#include "ModuleLoader\IModule.h"

class IEngineSystem : public IModule
{
public:

    // Get Engine Window 
    virtual void* GetEngineWindow( ) = 0;

    // Get Engine window sizes
    virtual void GetEngineWindowSize( float& width, float& height ) = 0;

    // Get screen sizes
    virtual void GetScreenSizes( float& width, float& height ) = 0;

    // Get elapsed time
    virtual float GetElapsedTime( ) = 0;

};

#endif // !__Engine_IEngineSystem_h__
