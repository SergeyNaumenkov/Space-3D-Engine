/*

    Copyright 2024 Sergey Naumenkov

    File: IBaseRenderer.h
    Description: Base renderer interface

    Date: 24/08/2024

*/

#ifndef __BaseRenderer_IBaseRenderer_h__
#define __BaseRenderer_IBaseRenderer_h__
#pragma once

#include "BaseRenderData.h"

#include "Mesh\IMesh.h"
#include "Mesh\IMesh2D.h"
#include "Math\Vector2D.h"

#include "MaterialSystem\MaterialData.h"

enum class RendererTag_t
{
    RenderTag_DirectX11,
    RenderTag_OpenGL,
    RenderTag_Metal,
    RenderTag_Vulkan,
};

class IBaseRenderer
{
public:

    // Initialize render system
    virtual bool Initialize( BaseRenderData_t& baseRenderData ) = 0;

    // Get Render Tag
    virtual RendererTag_t GetTag( ) = 0;

    // Get render device ( if Driver is supported )
    virtual void* GetDevice( ) = 0;

    // Start frame 
    virtual void StartFrame( ) = 0;

    // End Frame
    virtual void EndFrame( ) = 0;

    // Shutdown render system
    virtual void Shutdown( ) = 0;

    // Get 3D render mesh
    virtual IMesh* GetRenderMesh( ) = 0;
    
    // Get 2D render mesh
    virtual IMesh2D* GetRenderMesh2D( ) = 0;

    // Enable or disable Depth test
    virtual void SetDepthState( bool state ) = 0;

    // Enable or disable Blend state
    virtual void SetBlendState( bool state ) = 0;

    // Set Render scissors
    virtual void SetScissors( bool state, Vector2D pos, Vector2D size ) = 0;

    // Load material 
    virtual MaterialData_t* LoadMaterial( const char* materialPath, const char* materialName ) = 0;

    // Release material
    virtual void ReleaseMaterial( MaterialData_t* data ) = 0;
};

#endif // !__BaseRenderer_IBaseRenderer_h__
