/*

    Copyright 2024 Sergey Naumenkov

    File: Interfaces.h
    Description: Storage for all interfaces 

    Date: 02/08/2024

*/

#ifndef __InterfaceFactory_Interfaces_h__
#define __InterfaceFactory_Interfaces_h__
#pragma once

// Engine interface
#include "Engine/IEngineSystem.h"
extern IEngineSystem* g_EngineSystem;

// Input System
#include "InputSystem/IInputSystem.h"
extern IInputSystem* g_InputSystem;

// Shader System
#include "ShaderSystem/IShaderSystem.h"
extern IShaderSystem* g_ShaderSystem;

// Render system
#include "BaseRenderer\IBaseRenderer.h"
extern IBaseRenderer* g_RenderSystem;

// Material System
#include "MaterialSystem/IMaterialSystem.h"
extern IMaterialSystem* g_MaterialSystem;

// File System
#include "FileSystem/IFileSystem.h"
extern IFileSystem* g_FileSystem;

// Script System
#include "ScriptSystem/IScriptSystem.h"
extern IScriptSystem* g_ScriptSystem;

// Game dll's
#include "GameClient/IGameClient.h"
extern IGameClient* g_GameClient;

// Mist Graphics User Interface
#include "mGui\IMistGui.h"
extern IMistGui* g_mGui;

#endif // !__InterfaceFactory_Interfaces_h__
