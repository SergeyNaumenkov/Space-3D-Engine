/*

	Copyright 2024 Sergey Naumenkov

	File: PlayerMovement.cpp
	Description: Base logic for player movement

	Date: 07/08/2024

*/

#include "PlayerMovement.h"
#include "VectorUtils.h"

#include "InterfaceFactory/InterfaceFactory.h"
#include "Core/Debug.h"
#include "Math/Math.h"

const auto minVelocityValue = 1.1f;

/*
	Constructor
*/
PlayerMovement::PlayerMovement( )
{
	mPlayerMaxSpeed = 200;

	// Normal mode
	mPlayerMovementFlags = 0;
}

/*
	Destructor
*/
PlayerMovement::~PlayerMovement( )
{

}

/*
	Set position
*/
void PlayerMovement::SetPosition( Vector3Df newPosition )
{
	mPlayerPosition = newPosition;
}

/*
	Set view angle
*/
void PlayerMovement::SetViewAngle( Vector3Df newViewAngle )
{
	mPlayerViewAngle = newViewAngle;
}

/*
	Set movement flags
*/
void PlayerMovement::SetMoveFlags( int flag )
{
	mPlayerMovementFlags |= flag;
}

/*
	Is Player ducked
*/
bool PlayerMovement::IsPlayerDuck( )
{
	return mPlayerMovementFlags & 1;
}

/*
	Update movement
*/
void PlayerMovement::Update( )
{
	// Keyboard input
	ProcessKeyboardInput( );

	// Mouse Look
	ProcessMouseInput( );

	// Velocity logic
	ProcessOfVelocity( );

	//mPlayerVelocity.y -= 800 * g_EngineSystem->GetElapsedTime( ) * 2;

	// Last changes and game dll will send data into engine
	FinalMovement( );
}

/*
	Allow player change view angle by mouse
*/
void PlayerMovement::ProcessMouseInput( )
{
	auto mouse = g_InputSystem->GetMouseData( );

	// Pitch
	mPlayerViewAngle.x += 0.222f * ( mouse.mY * 1.0f * 1.5f ); // 1.0f - mouse sensetive; 1.5f adv value

	// Yaw
	mPlayerViewAngle.y -= 0.222f * ( mouse.mX * 1.0f * 1.5f ); // 1.0f - mouse sensetive; 1.5f adv value

	g_InputSystem->ResetMouseData( );

	// Clip our vertical view
	mPlayerViewAngle.x = Clamp( mPlayerViewAngle.x, -89, 89 );

	// Revert by horizontal
	if( mPlayerViewAngle.y > 180 )
		mPlayerViewAngle.y -= 360;
	else if( mPlayerViewAngle.y < -180 )
		mPlayerViewAngle.y += 360;

	//PrintMessage( "Player angle = %f x %f", mPlayerViewAngle.x, mPlayerViewAngle.y );
}

/*
	Process keyboard input
*/
void PlayerMovement::ProcessKeyboardInput( )
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
}

/*
	 Process the velocity of the player
*/
void PlayerMovement::ProcessOfVelocity( )
{
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
	auto frameTime = g_EngineSystem->GetElapsedTime( );

	// Calculate smooth stopping
	auto stopSpeedValue = currentSpeed * friction  * frameTime;
	auto newSpeed = currentSpeed - stopSpeedValue;
	if( newSpeed < 0 )
		newSpeed = 0;

	newSpeed /= currentSpeed;

	// Apply smooth stopping
	mPlayerVelocity.x *= newSpeed;
	mPlayerVelocity.y *= newSpeed;
	mPlayerVelocity.z *= newSpeed;

	// Clip velocity
	ClipVelocity( );
}

/*
	Clip velocity
*/
void PlayerMovement::ClipVelocity( )
{
	// clip velocity
	if( mPlayerVelocity.z > 0 && mPlayerVelocity.z < minVelocityValue ||
		mPlayerVelocity.z < 0 && mPlayerVelocity.z > -minVelocityValue )
	{
		mPlayerVelocity.z = 0;
	}

	if( mPlayerVelocity.x > 0 && mPlayerVelocity.x < minVelocityValue ||
		mPlayerVelocity.x < 0 && mPlayerVelocity.x > -minVelocityValue )
	{
		mPlayerVelocity.x = 0;
	}
}

/*
	Update render view 
*/
void PlayerMovement::FinalMovement( )
{
	// Direction Vectors
	Vector3Df forward, up, right;
	CalculateDirectionVectors( mPlayerViewAngle, forward, up, right );

	// Modif our position based on player velocity and rotation vectors
	// Reset up vector
	up = { 0.f,1.f,0.f };

	// Get Frame time
	auto frameTime = g_EngineSystem->GetElapsedTime( );

	// Update movement based on player velocity
	mPlayerPosition.x += ( forward.x * mPlayerVelocity.z + right.x * mPlayerVelocity.x + up.x * mPlayerVelocity.y ) * frameTime * 0.128f;
	mPlayerPosition.y += ( forward.y * mPlayerVelocity.z + right.y * mPlayerVelocity.x + up.y * mPlayerVelocity.y ) * frameTime * 0.128f;
	mPlayerPosition.z += ( forward.z * mPlayerVelocity.z + right.z * mPlayerVelocity.x + up.z * mPlayerVelocity.y ) * frameTime * 0.128f;
	
	// update view matrix based on player position
	//g_EngineSystem->GetEngineRender( )->UpdateViewMatrixData( mPlayerPosition, forward, up );
}
