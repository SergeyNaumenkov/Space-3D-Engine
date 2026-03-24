/*

    Copyright 2024 Sergey Naumenkov

    File: IMaterialSystem.h
    Description: Public interface for Material System

    Date: 03/08/2024

*/

#ifndef __MaterialSystem_IMaterialSystem_h__
#define __MaterialSystem_IMaterialSystem_h__
#pragma once

#include "MaterialData.h"

#include "ModuleLoader\IModule.h"

class IMaterialSystem : public IModule
{
public:

    // Load material from file
    virtual int LoadMaterial( const char* materialPath, const char* materialName ) = 0;

    // Create material from memory
    virtual bool CreateMaterialFromMemory( const char* materialName, int width, int height, void* data, int offset ) = 0;

    // Get Material by index
    virtual MaterialData_t* GetMaterial( int index ) = 0;

    // Get Material by name or file path
    virtual MaterialData_t* GetMaterial( const char* materialName, const char* materialPath = nullptr ) = 0;

    // Release material
    virtual bool ReleaseMaterial( const char* materialName ) = 0;
};

#endif // !__MaterialSystem_IMaterialSystem_h__
