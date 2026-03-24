/*

    Copyright 2024 Sergey Naumenkov

    File: SolidObjectsController.h
    Description: Controller for all solid objects. Stored it and render each frame

    Date: 16/08/2024

*/

#ifndef __SpaceLevelEditor_SolidObjectsController_h__
#define __SpaceLevelEditor_SolidObjectsController_h__
#pragma once

#include <vector>

#include "IMapSolidObject.h"
#include "3DCamera.h"

class SolidObjectsController
{
public:
	SolidObjectsController( );
	~SolidObjectsController( );

    // Get instance
    static SolidObjectsController* GetInstance( );

    // Initialize controller
    bool Initialize( );

    // add solid object
    void AddSolidObject( IMapSolidObject* object );

    // Remove solid object
    void RemoveSolidObject( IMapSolidObject* object );

    // Render solid objects
    void RenderSolidObjects( Render3DCamera* camera );

protected:

private:
    std::vector<IMapSolidObject*> mMapSolidObjects;

    int mSolidObjectsCount;
    int mVerticesTotalOffset;
    int mIndicesTotalOffset;

    int mSolidObjectsConstantBufferID;
};

#endif // !__SpaceLevelEditor_SolidObjectsController_h__
