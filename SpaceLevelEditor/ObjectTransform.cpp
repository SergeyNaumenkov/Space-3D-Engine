/*

	Copyright 2024 Sergey Naumenkov

	File: ObjectTransform.cpp
	Description: Transform object in the world space by mouse

	Date: 12/08/2024

*/

#include "Platform/Windows.h"

#include "ObjectTransform.h"

/*
	Constructor
*/
ObjectTransform::ObjectTransform( )
{
	mCurrentTransformMode = TransformMode_e::Mode_Move;
}

/*
	Destructor
*/
ObjectTransform::~ObjectTransform( )
{

}

/*
	Start transformation
*/
void ObjectTransform::Start( void* wnd )
{
	POINT pp;
	GetCursorPos( &pp );
	ScreenToClient( ( HWND )wnd, &pp );

	mCurrentMousePosition.x = pp.x;
	mCurrentMousePosition.y = pp.y;
}

/*
	Transform object position
*/
Vector3Df ObjectTransform::TransformPosition( TransformAxis_e axis, Vector3Df objectPosition, float transformSpeed )
{
	// Get mouse coords 
	int mouseX = mCurrentMousePosition.x - mOldMousePosition.x;
	int mouseY = mCurrentMousePosition.y - mOldMousePosition.y;

	// Change position
	Vector3Df position = { };

	// Select axis for transform
	switch( axis )
	{
		case TransformAxis_e::Axis_X:
			position = Vector3Df( -mouseX * transformSpeed, 0.0f, 0.0f );
			break;
		case TransformAxis_e::Axis_Y:
			position = Vector3Df( 0.0f, -mouseY * transformSpeed, 0.0f );
			break;
		case TransformAxis_e::Axis_Z:
			position = Vector3Df( 0.0f, 0.0f, -mouseY * transformSpeed );
			break;
		default:
			// it's never happen, because we using enum class
			// But this compiler must be shut up)
			position = objectPosition;
			break;
	}

	// Apply transform
	Vector3Df newPosition;
	newPosition.x = objectPosition.x + position.x;
	newPosition.y = objectPosition.y + position.y;
	newPosition.z = objectPosition.z + position.z;

	return position;
}

/*
	Transform object size
*/
Vector3Df ObjectTransform::TransformSize( TransformAxis_e axis, Vector3Df objectSize, float transformSpeed )
{
	// Get mouse coords 
	int mouseX = mCurrentMousePosition.x - mOldMousePosition.x;
	int mouseY = mCurrentMousePosition.y - mOldMousePosition.y;

	// Change position
	Vector3Df size = { };

	// Select axis for transform
	switch( axis )
	{
		case TransformAxis_e::Axis_X:
			size = Vector3Df( -mouseX * transformSpeed, 0.0f, 0.0f );
			break;
		case TransformAxis_e::Axis_Y:
			size = Vector3Df( 0.0f, -mouseY * transformSpeed, 0.0f );
			break;
		case TransformAxis_e::Axis_Z:
			size = Vector3Df( 0.0f, 0.0f, -mouseY * transformSpeed );
			break;
		default:
			// it's never happen, because we using enum class
			// But this compiler must be shut up)
			size = objectSize;
			break;
	}

	return size;
}

/*
	Change current transform mode
	By default it's move
*/
void ObjectTransform::SetTransformMode( TransformMode_e mode )
{
	mCurrentTransformMode = mode;
}

/*
	End transformation
*/
void ObjectTransform::End( )
{
	mOldMousePosition = mCurrentMousePosition;
}
