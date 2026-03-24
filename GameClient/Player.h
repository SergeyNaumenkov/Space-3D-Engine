/*

    Copyright 2024 Sergey Naumenkov

    File: Player.h
    Description: Main Player of game 

    Date: 07/08/2024

*/

#ifndef __GameClient_Player_h__
#define __GameClient_Player_h__
#pragma once

#include "PlayerMovement.h"

class Player
{
public:
	Player( );
	~Player( );

    // Init entity
    bool OnInit( );

    // Spawn player in the world
    void Spawn( Vector3Df position, Vector3Df angle );

    // Update entity player each frame
    void Update( );

private:
    PlayerMovement* mPlayerMovement;
};

#endif // !__GameClient_Player_h__
