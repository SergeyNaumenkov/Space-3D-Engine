/*

    Copyright 2024 Sergey Naumenkov

    File: EngineTime.cpp
    Description: Helper class for calculation of elapsed time between frames

    Date: 07/08/2024

*/

#include "EngineTime.h"

EngineTime::EngineTime( )
{
    // ?
    g_frequency = LARGE_INTEGER( );
    g_startTime = LARGE_INTEGER( );

    mEngineElapsedTime = 0.0f;
    mFrameTime = 0.0f;
}

EngineTime::~EngineTime( )
{

}

/*
    Initialize timer
*/
bool EngineTime::OnInit( )
{
    // Initialize timer
    QueryPerformanceFrequency( &g_frequency );
    QueryPerformanceCounter( &g_startTime );

    return true;
}

/*
    Start
*/
void EngineTime::Start( )
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter( &currentTime );

    // Calculate 
    mEngineElapsedTime = ( currentTime.QuadPart - g_startTime.QuadPart ) / ( float )g_frequency.QuadPart;

    // Update time
    g_startTime = currentTime;
}

float EngineTime::GetElapsedTime2( )
{
    return mEngineElapsedTime;
}

/*
    Get elapsed time
*/
float EngineTime::GetElapsedTime( )
{
    return mFrameTime;
}

/*
    Set frame time
*/
void EngineTime::SetFrameTime( float time )
{
    mFrameTime = time;
}
