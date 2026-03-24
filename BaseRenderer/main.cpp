/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: Redirector

	Date: 24/08/2024

*/

#include "IBaseRenderer.h"
#include "Core/StringTools.h"
#include "InterfaceFactory\InterfaceFactory.h"

class BaseRenderer
{
public:
	BaseRenderer( );
	~BaseRenderer( );

	IBaseRenderer* LoadRenderer( const char* renderName );

private:

};

BaseRenderer::BaseRenderer( )
{

}

BaseRenderer::~BaseRenderer( )
{

}

/*
	Describe this!
*/
IBaseRenderer* BaseRenderer::LoadRenderer( const char* renderName )
{
	if( CompareStrings( renderName, "DirectX11" ) )
	{
		auto renderer = GetClassFromDll<IBaseRenderer*>( "RenderDx11.dll", "IBaseRenderDirectX11" );
		return renderer;
	}
	else if( CompareStrings( renderName, "OpenGL" ) )
	{
		// Later
	}

	return nullptr;
}


extern "C"
{
	__declspec( dllexport ) IBaseRenderer* GetBaseRenderer( const char* renderName )
	{
		static BaseRenderer baseRenderer;
		return baseRenderer.LoadRenderer( renderName );
	}
}