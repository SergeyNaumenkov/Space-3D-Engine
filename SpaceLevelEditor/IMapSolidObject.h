/*

    Copyright 2024 Sergey Naumenkov

    File: IMapSolidObject.h
    Description: Shared interface for all solid objects in the world.
    Note: I think this system cam derived via 'Object pool'. I learning this pattern
          and maybe later implement it into Level editor

    Date: 16/08/2024

*/

#ifndef __SpaceLevelEditor_IMapSolidObject_h__
#define __SpaceLevelEditor_IMapSolidObject_h__
#pragma once

#include "BoundingBox.h"
#include "Math/Vector2D.h"

#include "Mesh\IMesh.h"

/*
    Shared interface for solid objects
*/
class IMapSolidObject
{
public:

    // Create object from bounding box
    virtual void CreateFromBoundingBox( BoundingBox_t& bbox ) = 0;

    // Render solid faces
    virtual void RenderSolidFaces( IMesh* mesh ) = 0;

};

#endif // !__SpaceLevelEditor_IMapSolidObject_h__
