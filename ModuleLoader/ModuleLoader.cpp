/*

    Copyright 2024 Sergey Naumenkov

    File: ModuleLoader.cpp
    Description: Load all modules and initialize
    Note: Same system i saw in Valve's source engine.

    Date: 25/08/2024

*/

#include "InterfaceFactory\InterfaceFactory.h"
#include "Core/Debug.h"

#include "ModuleLoader.h"

ModuleLoader::ModuleLoader( )
{

}

/*
    Load modules
*/
bool ModuleLoader::LoadModules( std::vector<ModuleData_t> modules )
{
    for( auto& module : modules )
    {
        // Get IModule functions from module
        IModule* mdl = GetClassFromDll<IModule*>( module.dllName, module.baseDllMethodName );
        if( !mdl )
        {
            PrintError( "Failed get IModule from %s!", module.dllName );
            return false;
        }

        mModules.push_back( mdl );
    }

    return true;
}

/*
    Pre init
*/
bool ModuleLoader::PreInit( )
{
    // Call PreInit from modules
    for( auto& module : mModules )
    {
        auto isInit = module->PreInit( );
        if( !isInit )
        {
            PrintMessage( "ModuleLoader-> PreInit is return false!" );
            return false;
        }
    }

    return true;
}

/*
    Init
*/
bool ModuleLoader::Initialize( )
{
    // Call Initialize from modules
    for( auto& module : mModules )
    {
        auto isInit = module->Initialize( );
        if( !isInit )
        {
            PrintMessage( "ModuleLoader-> Initialize is return false!" );
            return false;
        }
    }

    return true;
}

/*
    Start
*/
void ModuleLoader::Start( )
{
    // Only engine must be have start function!
    // Call Initialize from modules
    for( auto& module : mModules )
    {
        module->Start( );
    }

    // Engine is wanna sleep)
    // GN
}

/*
    Unload modules
*/
void ModuleLoader::UnloadModules( )
{
    // Call shutdown from modules
    for( auto& module : mModules )
    {
        module->Shutdown( );
    }

    // Unload all
    UnloadAllModules( );
}
