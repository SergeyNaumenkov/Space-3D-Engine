/*

	Copyright 2024 Sergey Naumenkov

	File: RenderScene.cpp
	Description: 3D Render Scene

	Date: 10/08/2024

*/

#include "RenderScene.h"

#include "InterfaceFactory/InterfaceFactory.h"
#include "Core/Debug.h"
#include "RayCast.h"

/*
	Constructor
*/
RenderScene::RenderScene( )
{
	mRenderCamera = nullptr;
}

/*
	Destructor
*/
RenderScene::~RenderScene( )
{
	delete mRenderCamera;
}

/*
	Initialize render scene
*/
bool RenderScene::Initialize( )
{
	// Initialize 3D camera
	mRenderCamera = new Render3DCamera( );
	mRenderCamera->Initialize( );

	// Load test material
	g_MaterialSystem->LoadMaterial( "materials\\dev1.dds", "dev1" );

	return true;
}

/*
	Render scene
*/
void RenderScene::Render( )
{
	mRenderCamera->Update( );
}

/*
	Add Cube to scene
*/
void RenderScene::AddCube( Vector3Df position, Vector3Df size )
{
	//mWorldPrimitives->AddCube( position, size );
}

/*
	Get camera
*/
Render3DCamera* RenderScene::GetCamera( )
{
	return mRenderCamera;
}

/*
	Try ray cast object, if mouse was pressed
	Detected object will be saved
*/
void RenderScene::TryRayCastObject( HWND wnd )
{
}
