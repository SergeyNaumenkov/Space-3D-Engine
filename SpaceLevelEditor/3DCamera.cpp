/*

	Copyright 2024 Sergey Naumenkov

	File: 3DCamera.cpp
	Description: 3D camera for editor

	Date: 10/08/2024

*/

#include "3DCamera.h"

#include "InterfaceFactory/InterfaceFactory.h"
#include "Core/Debug.h"
#include "Math/Math.h"

const int mPlayerMaxSpeed = 250;

/*
	Constructor
*/
Render3DCamera::Render3DCamera( )
{
	mAllowFreeMove = false;
}

/*
	Destructor
*/
Render3DCamera::~Render3DCamera( )
{

}

/*
	Initialize
*/
void Render3DCamera::Initialize( )
{
	mCameraPosition = { 0,0,-64 };

	// Initialize view matrix
	XMVECTOR Eye = XMVectorSet( mCameraPosition.x, mCameraPosition.y, mCameraPosition.z, 0.0f ); // position
	XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ); // look at
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	mViewMatrix = XMMatrixLookAtRH( Eye, At, Up );

	// Initialize projection matrix
	mProjectionMatrix = XMMatrixPerspectiveFovRH( XMConvertToRadians( 90 ),
												  1280.f / 720.f,
												  0.01f, 4096.f );
}

/*
	Update camera
*/
void Render3DCamera::Update( )
{
	if( g_InputSystem->IsKeyDown( Key_z ) )
	{
		mAllowFreeMove = !mAllowFreeMove;
	}

	if( mAllowFreeMove )
	{
		// Mouse look
		ProcessMouseInput( );

		// Keyboard movement
		ProcessKeyboardInput( );
	}

	// Update matrix 
	UpdateMatrix( );
}

/*
	Get camera position
*/
Vector3Df Render3DCamera::GetPosition( )
{
	return mCameraPosition;
}

/*
	Get camera view angle
*/
Vector3Df Render3DCamera::GetViewAngle( )
{
	return mCameraViewAngle;
}

/*
	Enable or disable camera input 
*/
void Render3DCamera::SetCameraInputState( bool state )
{
	mAllowFreeMove = state;
}

/*
	Get view matrix
*/
XMMATRIX Render3DCamera::GetViewMatrix( )
{
	return mViewMatrix;
}

/*
	Get projection matrix
*/
XMMATRIX Render3DCamera::GetProjectionMatrix( )
{
	return mProjectionMatrix;
}

/*
	Process mouse input
*/
void Render3DCamera::ProcessMouseInput( )
{
	auto mouse = g_InputSystem->GetMouseData( );

	// Pitch
	mCameraViewAngle.x += 0.222f * ( mouse.mY * 1.0f * 1.5f ); // 1.0f - mouse sensetive; 1.5f adv value

	// Yaw
	mCameraViewAngle.y -= 0.222f * ( mouse.mX * 1.0f * 1.5f ); // 1.0f - mouse sensetive; 1.5f adv value

	g_InputSystem->ResetMouseData( );

	// Clip our vertical view
	mCameraViewAngle.x = Clamp( mCameraViewAngle.x, -89, 89 );

	// Revert by horizontal
	if( mCameraViewAngle.y > 180 )
		mCameraViewAngle.y -= 360;
	else if( mCameraViewAngle.y < -180 )
		mCameraViewAngle.y += 360;

	// lock system cursor
	auto screenWidth = GetSystemMetrics( SM_CXSCREEN );
	auto screenHeight = GetSystemMetrics( SM_CYSCREEN );
	SetCursorPos( screenWidth / 2, screenHeight / 2 );
}

/*
	Process keyboard input
*/
void Render3DCamera::ProcessKeyboardInput( )
{
	// Forward
	if( g_InputSystem->IsKeyDown( Key_W ) )
	{
		mPlayerVelocity.z += mPlayerMaxSpeed;
	}

	// Backward
	if( g_InputSystem->IsKeyDown( Key_S ) )
	{
		mPlayerVelocity.z -= mPlayerMaxSpeed;
	}

	if( g_InputSystem->IsKeyDown( Key_D ) )
	{
		mPlayerVelocity.x -= mPlayerMaxSpeed;
	}

	if( g_InputSystem->IsKeyDown( Key_Space ) )
	{
		mPlayerVelocity.y += mPlayerMaxSpeed;
	}

	if( g_InputSystem->IsKeyDown( Key_A ) )
	{
		mPlayerVelocity.x += mPlayerMaxSpeed;
	}

	// Clip our speed by z
	mPlayerVelocity.z = Clamp( mPlayerVelocity.z, -mPlayerMaxSpeed, mPlayerMaxSpeed );

	// Clip our speed by x
	mPlayerVelocity.x = Clamp( mPlayerVelocity.x, -mPlayerMaxSpeed, mPlayerMaxSpeed );

	// Clip our speed by x
	mPlayerVelocity.y = Clamp( mPlayerVelocity.y, -mPlayerMaxSpeed, mPlayerMaxSpeed );

	// Apply friction
	// Get current player speed based on velocity
	auto currentSpeed = mPlayerVelocity.GetLength( );

	// if speed is null - player is freeze?
	if( currentSpeed <= 0 )
	{
		mPlayerVelocity = Vector3Df( );
		return;
	}

	// Get friction value
	const auto friction = 10.f;
	auto frameTime = 0.0031231231f;

	// Calculate smooth stopping
	auto stopSpeedValue = currentSpeed * friction * frameTime;
	auto newSpeed = currentSpeed - stopSpeedValue;
	if( newSpeed < 0 )
		newSpeed = 0;

	newSpeed /= currentSpeed;

	// Apply smooth stopping
	mPlayerVelocity.x *= newSpeed;
	mPlayerVelocity.y *= newSpeed;
	mPlayerVelocity.z *= newSpeed;

	// clip velocity
	if( mPlayerVelocity.z > 0 && mPlayerVelocity.z < 1.0f ||
		mPlayerVelocity.z < 0 && mPlayerVelocity.z > -1.0f )
	{
		mPlayerVelocity.z = 0;
	}

	if( mPlayerVelocity.x > 0 && mPlayerVelocity.x < 1.0f ||
		mPlayerVelocity.x < 0 && mPlayerVelocity.x > -1.0f )
	{
		mPlayerVelocity.x = 0;
	}
}

static void CalculateDirectionVectors( const Vector3Df& viewAngle, Vector3Df& forward, Vector3Df& up, Vector3Df& right )
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

static XMMATRIX CreateViewMatrix( const Vector3Df& viewPosition, const Vector3Df& forwardVector, const Vector3Df& upVector )
{
	// Get position vector
	auto eyePosition = XMVectorSet( viewPosition.x, viewPosition.y, viewPosition.z, 1.0f );

	// Get look at vector
	auto lookAt = XMVectorSet( forwardVector.x, forwardVector.y, forwardVector.z, 0.0f );

	// Get up vector
	auto up = XMVectorSet( upVector.x, upVector.y, upVector.z, 0.0f );

	// Create view matrix
	return XMMatrixLookAtRH( eyePosition, eyePosition + lookAt, up );
}

/*
	Update matrix
*/
void Render3DCamera::UpdateMatrix( )
{
	// Direction Vectors
	Vector3Df forward, up, right;
	CalculateDirectionVectors( mCameraViewAngle, forward, up, right );

	// Modif our position based on player velocity and rotation vectors
	// Reset up vector
	up = { 0.f,1.f,0.f };

	// Get Frame time
	auto frameTime = 0.00312312f;

	// Update movement based on player velocity
	mCameraPosition.x += ( forward.x * mPlayerVelocity.z + right.x * mPlayerVelocity.x + up.x * mPlayerVelocity.y ) * frameTime * 0.128f;
	mCameraPosition.y += ( forward.y * mPlayerVelocity.z + right.y * mPlayerVelocity.x + up.y * mPlayerVelocity.y ) * frameTime * 0.128f;
	mCameraPosition.z += ( forward.z * mPlayerVelocity.z + right.z * mPlayerVelocity.x + up.z * mPlayerVelocity.y ) * frameTime * 0.128f;

	// Create matrix
	mViewMatrix = CreateViewMatrix( mCameraPosition, forward, up );
}
