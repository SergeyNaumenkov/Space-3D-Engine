/*

    Copyright 2024 Sergey Naumenkov

    File: VectorUtils.h
    Description: Util functions for Vector all dimensions

    Date: 08/08/2024

*/

#ifndef __GameClient_VectorUtils_h__
#define __GameClient_VectorUtils_h__
#pragma once

#include <DirectXMath.h>
using namespace DirectX;

#include "Math/Vector3D.h"

// Get Forward, Right and Up vectors from ViewAngle vector
inline void CalculateDirectionVectors( const Vector3Df& viewAngle, Vector3Df& forward, Vector3Df& up, Vector3Df& right )
{
    // Get pitch, Yaw and Roll values
    auto pitch = XMConvertToRadians( viewAngle.x ); 
    auto yaw = XMConvertToRadians( viewAngle.y ); 
    auto roll = XMConvertToRadians( viewAngle.z ); 

    // Get rotation matrices
    auto pitchMatrix = XMMatrixRotationX( pitch );
    auto yawMatrix = XMMatrixRotationY( yaw );
    auto rollMatrix = XMMatrixRotationZ( roll );

    // Combine matrices
    auto rotationMatrix = rollMatrix * pitchMatrix * yawMatrix;

    // Directions
    auto forwardVec = XMVectorSet( 0, 0, 1, 0 );
    auto upVec = XMVectorSet( 0, 1, 0, 0 );
    auto rightVec = XMVectorSet( 1, 0, 0, 0 );

    // Apply rotation
    forwardVec = XMVector3TransformCoord( forwardVec, rotationMatrix );
    upVec = XMVector3TransformCoord( upVec, rotationMatrix );
    rightVec = XMVector3TransformCoord( rightVec, rotationMatrix );

    // Save our vectors
    forward.x = XMVectorGetX( forwardVec );
    forward.y = XMVectorGetY( forwardVec );
    forward.z = XMVectorGetZ( forwardVec );

    up.x = XMVectorGetX( upVec );
    up.y = XMVectorGetY( upVec );
    up.z = XMVectorGetZ( upVec );

    right.x = XMVectorGetX( rightVec );
    right.y = XMVectorGetY( rightVec );
    right.z = XMVectorGetZ( rightVec );
}

#endif // !__GameClient_VectorUtils_h__
