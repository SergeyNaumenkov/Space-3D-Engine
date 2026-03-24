/*

    Copyright 2024 Sergey Naumenkov

    File: Player.cpp
    Description: Main Player of game

    Date: 07/08/2024

*/

#include "Player.h"
#include "InterfaceFactory/InterfaceFactory.h"

/*
    Constructor
*/
Player::Player( )
{
    mPlayerMovement = nullptr;
}

/*
    Destructor
*/
Player::~Player( )
{
    delete mPlayerMovement;
}

/*
    Init entity
*/
bool Player::OnInit( )
{
    mPlayerMovement = new PlayerMovement( );

    return true;
}

/*
    Spawn player in the world
*/
void Player::Spawn( Vector3Df position, Vector3Df angle )
{
    mPlayerMovement->SetPosition( position );
    mPlayerMovement->SetViewAngle( angle );
}

/*
    Update entity player each frame
*/
void Player::Update( )
{
    // Update movement
    mPlayerMovement->Update( );
}
