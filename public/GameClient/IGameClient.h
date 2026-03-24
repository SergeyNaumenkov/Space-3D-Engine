/*

    Copyright 2024 Sergey Naumenkov

    File: IGameClient.h
    Description: Public interface for game client

    Date: 04/08/2024

*/

#ifndef __GameClient_IGameClient_h__
#define __GameClient_IGameClient_h__
#pragma once

#include "Map/LevelMapDefs.h"

class IGameClient
{
public:

    // Initialize game client
    virtual bool OnInit( ) = 0;

    // Start game client 
    virtual void StartGame( ) = 0;

    // Shutdown game client 
    virtual void Shutdown( ) = 0;

};
#endif // !__GameClient_IGameClient_h__
