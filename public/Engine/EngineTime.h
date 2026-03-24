/*

    Copyright 2024 Sergey Naumenkov

    File: EngineTime.h
    Description: Helper class for calculation of elapsed time between frames

    Date: 07/08/2024

*/

#ifndef __Engine_EngineTime_h__
#define __Engine_EngineTime_h__
#pragma once

#include "Platform/Windows.h"

class EngineTime
{
public:
	EngineTime( );
	~EngineTime( );

    // Initialize timer
    bool OnInit( );

    // Start
    void Start( );

    // Get elapsed time
    float GetElapsedTime2( );
    float GetElapsedTime( );

    // Set frame time
    void SetFrameTime( float time );

private:
    LARGE_INTEGER g_frequency;
    LARGE_INTEGER g_startTime;

    float mEngineElapsedTime;
    float mFrameTime;
};

#endif // !__Engine_EngineTime_h__
