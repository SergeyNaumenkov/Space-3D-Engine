/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: Input system for all modules

	Date: 05/08/2024

*/

#include <windowsx.h>
#include <queue>
#include <mutex>
#include <unordered_set>

#include "Platform/Windows.h"
#include "IInputSystem.h"

#include "InterfaceFactory/InterfaceFactory.h"
#include "Core/Debug.h"
#include "Math/Vector2D.h"

struct RawInputEvent_t
{
	unsigned int msg;
	long long firstParam;
	long long secondParam;
};

class InputSystem : public IInputSystem
{
public:
	InputSystem( );
	~InputSystem( );

	// Pre initialize module
	virtual bool PreInit( ) override;

	// Initialize module
	virtual bool Initialize( ) override;

	// Start 
	virtual void Start( ) { };

	// Shutdown module
	virtual void Shutdown( ) override;

	// Register window event
	virtual void RegisterMessage( unsigned int msg, long long firstParam, long long secondParam );

	// Enable raw input
	virtual bool EnableRawInput( void* wnd );

	// Process Buffered messages
	virtual void ProcessBufferedMessages( );

	// Get Last event
	virtual InputDataEvent_t GetLastEvent( );

	// Get mouse data
	virtual MouseEventData_t GetMouseData( );

	// Reset mouse data
	virtual void ResetMouseData( );

	// Is Keyboard key pressed
	virtual bool IsKeyDown( int keyCode );

	// Get mouse position ( not raw )
	virtual Vector2D GetMousePosition( );

	// Copy window ptr
	virtual void SetWindowPtr( void* wnd );

private:
	InputDataEvent_t mLastInputData;

	bool mRawInputSupported;

	std::unordered_set<long long> mPressedKeys;
	std::queue<RawInputEvent_t> mInputBuffer;
	std::mutex mBufferMutex;

	void* mWindow;
};

InputSystem::InputSystem( )
{
	mLastInputData = { };
	mRawInputSupported = false;
}

InputSystem::~InputSystem( )
{

}

/*
	Pre initialize module
*/
bool InputSystem::PreInit( )
{
	// Load engine
	g_EngineSystem = GetClassFromDll<IEngineSystem*>( "Engine.dll", "EngineSystemMainV1" );
	if( !g_EngineSystem )
	{
		return false;
	}

	return true;
}

/*
	Initialize module
*/
bool InputSystem::Initialize( )
{

	return true;
}

/*
	Shutdown module
*/
void InputSystem::Shutdown( )
{

}

/*
	Register window event
*/
void InputSystem::RegisterMessage( unsigned int msg, long long firstParam, long long secondParam )
{
	// We're shouldn't store mouse data in queue. 
	// because it's will cause a delay in mouse input
	switch( msg )
	{
		case WM_INPUT:
		{
			UINT dwSize = sizeof( RAWINPUT );
			static BYTE lpb[ sizeof( RAWINPUT ) ];

			// GetRaw Input data
			GetRawInputData( reinterpret_cast< HRAWINPUT >( secondParam ), RID_INPUT, lpb, &dwSize, sizeof( RAWINPUTHEADER ) );

			auto rawInput = reinterpret_cast< RAWINPUT* >( lpb );
			if( rawInput->header.dwType == RIM_TYPEMOUSE ) // Raw Mouse Data
			{
				mLastInputData.mEventType = InputEvent_MouseMove;
				mLastInputData.mMouseData.mAdditionalKey = rawInput->data.mouse.ulButtons; // later
				mLastInputData.mMouseData.mX += static_cast< int >( rawInput->data.mouse.lLastX );
				mLastInputData.mMouseData.mY += static_cast< int >( rawInput->data.mouse.lLastY );
			}
			return; // Cancel register this in queue
		}
	}

	// Others events like as Keyboard keys
	std::lock_guard<std::mutex> lock( mBufferMutex );
	mInputBuffer.push( { msg,firstParam, secondParam } );
}

/*
	Enable raw input
*/
bool InputSystem::EnableRawInput( void* wnd )
{
	// Right now, init Raw Input only for mouse,
	// Skip Keyboard
	RAWINPUTDEVICE devices[ 1 ] = { };
	devices[ 0 ].usUsagePage = 0x01;
	devices[ 0 ].usUsage = 0x02;
	devices[ 0 ].dwFlags = RIDEV_INPUTSINK;    // adds mouse and also ignores legacy mouse messages <- From Microsoft Docs
	devices[ 0 ].hwndTarget = static_cast< HWND >( wnd );

	// Register Raw Input
	auto isRegistered = RegisterRawInputDevices( devices, ARRAYSIZE( devices ), sizeof( devices[ 0 ] ) );
	if( !isRegistered )
	{
		PrintError( "Input System: Raw Input isn't support by your  OS!" );
		mRawInputSupported = false;
	}

	mRawInputSupported = true;
	return mRawInputSupported;
}

/*
	Process Buffered messages
*/
void InputSystem::ProcessBufferedMessages( )
{
	std::lock_guard<std::mutex> lock( mBufferMutex );

	while( !mInputBuffer.empty( ) )
	{
		auto inputEventData = mInputBuffer.front( );
		mInputBuffer.pop( );

		switch( inputEventData.msg )
		{
			// Mouse buttons
			case WM_LBUTTONDOWN:
				mLastInputData.mEventType = InputEvent_MouseLeftButtonDown;
				mLastInputData.mMouseData.mAdditionalKey = inputEventData.firstParam; // if pressed ctrl ( for exam ) with left mouse
				break;

			case WM_LBUTTONUP:
				mLastInputData.mEventType = InputEvent_MouseLeftButtonUp;
				mLastInputData.mMouseData.mAdditionalKey = inputEventData.firstParam;
				break;

			case WM_RBUTTONDOWN:
				mLastInputData.mEventType = InputEvent_MouseRightButtonDown;
				mLastInputData.mMouseData.mAdditionalKey = inputEventData.firstParam;
				break;

			case WM_RBUTTONUP:
				mLastInputData.mEventType = InputEvent_MouseRightButtonUp;
				mLastInputData.mMouseData.mAdditionalKey = inputEventData.firstParam;
				break;

				// Mouse move( without raw input )
			/*case WM_MOUSEMOVE:
			{
				mLastInputData.mEventType = InputEvent_MouseMove;
				mLastInputData.mMouseData.mAdditionalKey = inputEventData.firstParam;
				mLastInputData.mMouseData.mX = GET_X_LPARAM( inputEventData.secondParam );
				mLastInputData.mMouseData.mY = GET_Y_LPARAM( inputEventData.secondParam );
			}
			break;*/

			case WM_KEYDOWN:
				mLastInputData.mEventType = InputEvent_KeyDown;
				mLastInputData.mKeyboardData.mKey = inputEventData.firstParam;
				mLastInputData.mKeyboardData.mSecondKey = inputEventData.secondParam;
				mPressedKeys.insert( inputEventData.firstParam );
				break;

			case WM_KEYUP:
				mLastInputData.mEventType = InputEvent_KeyUp;
				mLastInputData.mKeyboardData.mKey = inputEventData.firstParam;
				mLastInputData.mKeyboardData.mSecondKey = inputEventData.secondParam;
				mPressedKeys.erase( inputEventData.firstParam );
				break;

			default:
				// Unknown message
				break;
		}
	}
}

/*
	Get Last event
*/
InputDataEvent_t InputSystem::GetLastEvent( )
{
	return mLastInputData;
}

/*
	Get mouse data
*/
MouseEventData_t InputSystem::GetMouseData( )
{
	return mLastInputData.mMouseData;
}

/*
	Reset mouse data
*/
void InputSystem::ResetMouseData( )
{
	mLastInputData.mMouseData.mX = 0;
	mLastInputData.mMouseData.mY = 0;
	mLastInputData.mMouseData.mAdditionalKey = 0;
}

/*
	Is Keyboard key pressed
*/
bool InputSystem::IsKeyDown( int keyCode )
{
	std::lock_guard<std::mutex> lock( mBufferMutex );
	return mPressedKeys.find( keyCode ) != mPressedKeys.end( );
}

/*
	Get mouse position ( not raw )
*/
Vector2D InputSystem::GetMousePosition( )
{
	POINT pp;
	GetCursorPos( &pp );
	ScreenToClient( reinterpret_cast< HWND >( mWindow ), &pp );

	if( pp.x < 0 || pp.y < 0 ||
		pp.x >= 1280 || pp.y >= 720 )
	{
		return { 0, 0 };  
	}

	return Vector2D( pp.x, pp.y );
}

/*
	Copy window ptr
*/
void InputSystem::SetWindowPtr( void* wnd )
{
	mWindow = wnd;
}
SaveDllClass( InputSystem, IInputSystemV1 )