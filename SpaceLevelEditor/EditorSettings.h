/*

    Copyright 2024 Sergey Naumenkov

    File: EditorSettings.h
    Description: global space class for storing the editor settings
    Note: This class based on the Singleton pattern.

    Date: 20/08/2024

*/

#ifndef __SpaceLevelEditor_EditorSettings_h__
#define __SpaceLevelEditor_EditorSettings_h__
#pragma once

class EditorSettings
{
public:
	
    // Get instance on the class
    EditorSettings& GetInstance( );

private:
    EditorSettings( );
    ~EditorSettings( );
};

#endif // !__SpaceLevelEditor_EditorSettings_h__
