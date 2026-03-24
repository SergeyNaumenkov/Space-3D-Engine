/*

	Copyright 2024 Sergey Naumenkov

	File: Color.h
	Description: Color

	Date: 04/08/2024

*/

#ifndef __Public_Color_h__
#define __Public_Color_h__
#pragma once

enum class ColorComponent_t
{
	Color_Red,
	Color_Green,
	Color_Blue,
	Color_Alpha
};

/*
	Color class
*/
class Color
{
public:
	// Empty
	Color( )
	{
		m_rgba[ 0 ] = m_rgba[ 1 ] = m_rgba[ 2 ] = m_rgba[ 3 ] = 0;
	}

	// Without alpha
	Color( int r, int g, int b )
	{
		m_rgba[ 0 ] = ( unsigned char )r;
		m_rgba[ 1 ] = ( unsigned char )g;
		m_rgba[ 2 ] = ( unsigned char )b;
		m_rgba[ 3 ] = 0;
	}

	// Full rgba
	Color( int r, int g, int b, int a )
	{
		m_rgba[ 0 ] = ( unsigned char )r;
		m_rgba[ 1 ] = ( unsigned char )g;
		m_rgba[ 2 ] = ( unsigned char )b;
		m_rgba[ 3 ] = ( unsigned char )a;
	}

	// Get color component
	int GetComponent( ColorComponent_t component )
	{
		switch( component )
		{
			case ColorComponent_t::Color_Red:
				return static_cast< int >( m_rgba[ 0 ] );
				break;
			case ColorComponent_t::Color_Green:
				return static_cast< int >( m_rgba[ 1 ] );
				break;
			case ColorComponent_t::Color_Blue:
				return static_cast< int >( m_rgba[ 2 ] );
				break;
			case ColorComponent_t::Color_Alpha:
				return static_cast< int >( m_rgba[ 3 ] );
				break;
			default:
				break;
		}

		// invalid compnent!
		return 0;
	}

	// Convert rgba to dword
	unsigned long ToLong( )
	{
		return ( ( m_rgba[ 3 ] * 256 + m_rgba[ 0 ] ) * 256 + m_rgba[ 1 ] ) * 256 + m_rgba[ 2 ];
	}

private:
	unsigned char m_rgba[ 4 ];
};

#endif // !__Public_Color_h__
