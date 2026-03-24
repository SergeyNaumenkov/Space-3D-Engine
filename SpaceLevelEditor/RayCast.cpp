/*

    Copyright 2024 Sergey Naumenkov

    File: RayCast.cpp
    Description: Ray cast

    Date: 11/08/2024

*/

#include <iostream>

#include "RayCast.h"

/*
    Constructor
*/
RayCast::RayCast( )
{

}

/*
    Destructor
*/
RayCast::~RayCast( )
{

}

/*
    Convert screen mouse position to world position
*/
XMVECTOR RayCast::GetWorldMousePosition( int mouseX, int mouseY, int screenWidth, int screenHeight, Render3DCamera* camera )
{
    // Normalize mouse coords
    float ndcX = ( 2.0f * mouseX ) / screenWidth - 1.0f;
    float ndcY = 1.0f - ( 2.0f * mouseY ) / screenHeight;
    auto rayRaw = XMVectorSet( ndcX, ndcY, 1.0f, 1.0f );

    // Inverse matrices
    auto invView = XMMatrixInverse( nullptr, camera->GetViewMatrix( ) );
    auto invProj = XMMatrixInverse( nullptr, camera->GetProjectionMatrix( ) );

    // Convert ray to world
    auto rayWorld = XMVector3TransformCoord( rayRaw, invProj ); // by proj
    rayWorld = XMVector3TransformCoord( rayWorld, invView ); // by view

    // Normalize vector
    rayWorld = XMVector3Normalize( rayWorld );

    return rayWorld;
}

/*
    Cast ray in to rayDirection
    If box is detected - return true
    Otherwise - false
    distanceToObject: Distance from ray origin to object
*/
bool RayCast::CastRayForBox( XMVECTOR rayOrigin, XMVECTOR rayDirection, Vector3Df bboxMin, Vector3Df bboxMax, float& distanceToObject )
{
    auto dMin = 0.0f;
    auto dMax = FLT_MAX;

    // Process by three axis ( x,y,z )
    for( int i = 0; i < 3; i++ )
    {
        // Get origin component for current axis
        auto origin = XMVectorGetByIndex( rayOrigin, i );

        // Get ray direction component for current axis
        auto dir = XMVectorGetByIndex( rayDirection, i );

        // Get axis component from boundingBox
        auto min = bboxMin[ i ];
        auto max = bboxMax[ i ];

        // calculate
        auto t1 = ( min - origin ) / dir;
        auto t2 = ( max - origin ) / dir;

        if( t1 > t2 )
            std::swap( t1, t2 );

        dMin = std::max( dMin, t1 );
        dMax = std::min( dMax, t2 );

        // No intersections
        if( dMin > dMax )
            return false;
    }

    // Intersection detected
    // Update distance from ray start to object
    distanceToObject = dMin;
    return true;
}
