/*

	Copyright 2024 Sergey Naumenkov

	File: Display.cpp
	Description: Stores information about all display modes

	Date: 06/09/2024

*/

#include <algorithm>

#include "Display.h"
#include "Core/Debug.h"

Display::Display( )
{
	mDxgiModes = nullptr;

	mDxgiModesCount = 0;
	mModesCount = 0;
}

/*
	Initialize and get display modes
*/
bool Display::Initialize( IDXGIFactory2*& dxgiFactory )
{
	IDXGIAdapter1* dxgiAdapter = nullptr;

	// Get adapter
	auto res = dxgiFactory->EnumAdapters1( 0, &dxgiAdapter );
	if( FAILED( res ) )
	{
		PrintError( "Failed get dxgi adapter!" );
		return false;
	}

	// Get output modes
	IDXGIOutput* dxgiOutput = nullptr;
	res = dxgiAdapter->EnumOutputs( 0, &dxgiOutput );
	if( FAILED( res ) )
	{
		PrintError( "Failed get dxgi modes!" );
		return false;
	}

	DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;

	// Get the number of elements
	res = dxgiOutput->GetDisplayModeList( format, 0, &mDxgiModesCount, NULL );
	if( FAILED( res ) )
	{
		PrintError( "Failed get dxgi display modes list(count)!" );
		return false;
	}

	mDxgiModes = new DXGI_MODE_DESC[ mDxgiModesCount ];

	// Get the list
	res = dxgiOutput->GetDisplayModeList( format, 0, &mDxgiModesCount, mDxgiModes );
	if( FAILED( res ) )
	{
		PrintError( "Failed get dxgi display modes list!" );
		return false;
	}

	// Parse 
	ParseModes( );

	// Cleanup old information
	if( mDxgiModes )
	{
		delete[ ] mDxgiModes;
		mDxgiModes = nullptr;
	}

	mDxgiModesCount = 0;
	

	return true;
}

/*
	Get mode
*/
const DisplayMode_t& Display::GetDisplayMode( int index )
{
	return mModes[ index ];
}

/*
	Get total count of modes
*/
const int Display::GetModesCount( )
{
	return mModesCount;
}

/*
	Shutdown
*/
void Display::Shutdown( )
{
	if( mModes.size( ) > 0 )
	{
		mModes.ClearAll( );
	}
}

/*
	Parse modes
*/
void Display::ParseModes( )
{
	for( unsigned int i = 0; i < mDxgiModesCount; i++ )
	{
		auto dxgiMode = mDxgiModes[ i ];

		// Remove duplicates ( if exist )
		bool isDuplicate = false;
		for( int y = 0; y < mModes.size( ); y++ )
		{
			if( mModes[ y ].mWidth == dxgiMode.Width &&
				mModes[ y ].mHeight == dxgiMode.Height &&
				mModes[ y ].mRefreshRate == dxgiMode.RefreshRate.Numerator / dxgiMode.RefreshRate.Denominator )
			{
				isDuplicate = true;
			}
		}

		if( !isDuplicate )
		{
			DisplayMode_t mode = { };
			mode.mWidth = dxgiMode.Width;
			mode.mHeight = dxgiMode.Height;
			mode.mRefreshRate = dxgiMode.RefreshRate.Numerator / dxgiMode.RefreshRate.Denominator;

			mModes.AddToBack( mode );
		}
	}

	mModesCount = mModes.size( );
}