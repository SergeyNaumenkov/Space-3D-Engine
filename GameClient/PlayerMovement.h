/*

	Copyright 2024 Sergey Naumenkov

	File: PlayerMovement.h
	Description: Base logic for player movement

	Date: 07/08/2024

*/

#ifndef __GameCliet_PlayerMovement_h__
#define __GameCliet_PlayerMovement_h__
#pragma once

#include "Math/Vector3D.h"

class PlayerMovement
{
public:
	PlayerMovement( );
	~PlayerMovement( );

	// Set position
	void SetPosition( Vector3Df newPosition );

	// Set view angle
	void SetViewAngle( Vector3Df newViewAngle );

	// Set movement flags
	void SetMoveFlags( int flag );

	// Is Player ducked
	bool IsPlayerDuck( );

	// Update movement
	void Update( );

protected:

	// Process mouse input
	void ProcessMouseInput( );

	// Process keyboard input
	void ProcessKeyboardInput( );

	// Process the velocity of the player
	void ProcessOfVelocity( );

	// Clip velocity
	void ClipVelocity( );

	// Update render view 
	void FinalMovement( );

private:
	Vector3Df mPlayerPosition;
	Vector3Df mPlayerViewAngle;
	Vector3Df mPlayerVelocity;

	float mPlayerMaxSpeed;
	int mPlayerMovementFlags;
};

#endif // !__GameCliet_PlayerMovement_h__
