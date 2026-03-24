/*

    Copyright 2024 Sergey Naumenkov

    File: EditorSettings.cpp
    Description: global space class for storing the editor settings
    Note: This class based on the Singleton pattern.

    Date: 20/08/2024

*/

#include "EditorSettings.h"

/*
    Get instance on the class
*/
EditorSettings& EditorSettings::GetInstance( )
{
    static EditorSettings instance;
    return instance;
}

/*
    Private constructor
*/
EditorSettings::EditorSettings( )
{

}

/*
     Private destructor
*/
EditorSettings::~EditorSettings( )
{

}
