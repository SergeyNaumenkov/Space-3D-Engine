/*

	Copyright 2024 Sergey Naumenkov

	File: MapBox.h
	Description: Box primitive
	Note: Shared class for stores all map faces

	Date: 15/08/2024

*/

#ifndef __SpaceLevelEditor_MapBox_h__
#define __SpaceLevelEditor_MapBox_h__
#pragma once

#include <vector>

#include "IMapSolidObject.h"
#include "Math/Vector2D.h"


struct SolidBoxFace_t
{
	std::vector<Vector3Df> mVertex;
	std::vector<Vector2Df> mVertexUV;
	Vector3Df normal;
	int mTotalVerticesCount;

	int mMaterialId;
};

class MapSolidBox : public IMapSolidObject
{
public:
	MapSolidBox( );
	~MapSolidBox( );

	// Create map box (solid)
	virtual void CreateFromBoundingBox( BoundingBox_t& bbox );

	// Render solid faces
	virtual void RenderSolidFaces( IMesh* mesh );

protected:


private:
	std::vector<SolidBoxFace_t> mBoxFaces;
};

#endif // !__SpaceLevelEditor_MapBox_h__
