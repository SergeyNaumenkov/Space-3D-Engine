/*

    Copyright 2024 Sergey Naumenkov

    File: ModuleLoader.h
    Description: Load all modules and initialize
    Note: Same system i saw in Valve's source engine.

    Date: 25/08/2024

*/

#ifndef __ModuleLoader_ModuleLoader_h__
#define __ModuleLoader_ModuleLoader_h__
#pragma once

#include <vector>
#include "IModule.h"

struct ModuleData_t
{
    const char* dllName;
    const char* baseDllMethodName;
};

class ModuleLoader
{
public:
	ModuleLoader( );

    // Load modules
    bool LoadModules( std::vector<ModuleData_t>modules );

    // Pre init
    bool PreInit( );

    // Init
    bool Initialize( );

    // Start
    void Start( );

    // Unload modules
    void UnloadModules( );

private:
    std::vector<IModule*> mModules;
};

#endif // !__ModuleLoader_ModuleLoader_h__
