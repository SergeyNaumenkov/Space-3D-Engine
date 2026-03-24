/*

    Copyright 2024 Sergey Naumenkov

    File: RenderDevicesInternal.h
    Description: Render devices copy for internal uses

    Date: 25/08/2024

*/

#ifndef __RenderDx11_RenderDevicesInternal_h__
#define __RenderDx11_RenderDevicesInternal_h__
#pragma once

#include <d3d11_1.h>

#include "Core/Debug.h"

struct RenderDevicesInternal_t
{
    RenderDevicesInternal_t( )
    {
        mTotalDrawCalls = 0;
    }

    ID3D11Device1* mDevice;
    ID3D11DeviceContext1* mDeviceContext;

    void DrawIndexed( UINT indexCount, UINT startIndex, INT  baseVertex )
    {
        mDeviceContext->DrawIndexed( indexCount, startIndex, baseVertex );
        mTotalDrawCalls++;
    }

    void DrawInfoDrawCalls( )
    {
        PrintMessage( "Total draw Calls: %i", mTotalDrawCalls );

        // Reset for new frame 
        mTotalDrawCalls = 0;
    }

private:
    int mTotalDrawCalls;
};

extern RenderDevicesInternal_t*& GetRenderDevicesInternal( );

#endif // !__RenderDx11_RenderDevicesInternal_h__
