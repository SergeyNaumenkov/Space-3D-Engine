/*

    Copyright 2024 Sergey Naumenkov

    File: Interfaces.cpp
    Description: Storage for all interfaces

    Date: 02/08/2024

*/

#include "Interfaces.h"

// Engine interface
IEngineSystem* g_EngineSystem = nullptr;

// Input System interface
IInputSystem* g_InputSystem = nullptr;

// File System interface
IFileSystem* g_FileSystem = nullptr;

// Script System interface
IScriptSystem* g_ScriptSystem = nullptr;

// Shader System Interface
IShaderSystem* g_ShaderSystem = nullptr;

// Render system interface
IBaseRenderer* g_RenderSystem = nullptr;

// Material System interface
IMaterialSystem* g_MaterialSystem = nullptr;

// Game client interface
IGameClient* g_GameClient = nullptr;

// Mist Graphics User Interface
IMistGui* g_mGui = nullptr;