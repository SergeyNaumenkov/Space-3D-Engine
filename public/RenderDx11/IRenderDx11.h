/*

    Copyright 2024 Sergey Naumenkov

    File: IRenderDx11
    Description: Public interface for DirectX 11 Renderer

    Date: 24/08/2024

*/

#ifndef __RenderDx11_IRenderDx11_h__
#define __RenderDx11_IRenderDx11_h__
#pragma once

#include "BaseRenderData.h"

class IRenderDx11
{
public:

    // Initialize system
    virtual bool Initialize( BaseRenderData_t& renderData ) = 0;


    // Shutdown system
    virtual void Shutdown( ) = 0;
};

#endif // !__RenderDx11_IRenderDx11_h__
