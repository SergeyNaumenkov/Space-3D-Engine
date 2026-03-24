/*

    Copyright 2024 Sergey Naumenkov

    File: IModule.h
    Description: Public interface for all modules

    Date: 25/08/2024

*/

#ifndef __ModuleLoader_IModule_h__
#define __ModuleLoader_IModule_h__
#pragma once

class IModule
{
public:

    // Pre initialize module
    virtual bool PreInit( ) = 0;

    // Initialize module
    virtual bool Initialize( ) = 0;

    // Start 
    virtual void Start( ) = 0;

    // Shutdown module
    virtual void Shutdown( ) = 0;

};

#endif // !__ModuleLoader_IModule_h__
