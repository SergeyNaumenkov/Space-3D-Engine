/*

	Copyright 2024 Sergey Naumenkov

	File: Vector3D.h
	Description: Vector3D implementation

	Date: 04/08/2024

*/

#ifndef __Math_Vector3D_h__
#define __Math_Vector3D_h__
#pragma once

#include <cmath>

// Vector3D based on INT variables
struct Vector3D
{
	Vector3D( )
	{
		x = y = z = 0;
	}

	Vector3D( int _x, int _y, int _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}

	int x;
	int y;
	int z;
};

// Vector3D based on FLOAT variables
struct Vector3Df
{
	Vector3Df( )
	{
		x = y = z = 0.0f;
	}

	Vector3Df( float _x, float _y, float _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}

	float GetLength( )
	{
		return sqrt(x * x + y * y + z * z );
	}

	float Dot( const Vector3Df& other ) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vector3Df& operator+=( const Vector3Df& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3Df operator+( const Vector3Df& other ) const
	{
		return Vector3Df( x + other.x, y + other.y, z + other.z );
	}

	Vector3Df operator*( float value ) const
	{
		return Vector3Df( x * value, y * value, z * value );
	}

	void Normalize( )
	{
		float length = GetLength( );
		if( length != 0.0f )
		{
			x /= length;
			y /= length;
			z /= length;
		}
	}

	bool operator==( Vector3Df v )
	{
		if( x == v.x && y == v.y && z == v.z )
			return true;

		return false;
	}

	/*float operator[]( int index )
	{
		if( index == 0 )
			return x;

		if( index == 1 )
			return y;

		return z;
	}*/

	float& operator[]( int index )
	{
		if( index == 0 )
			return x;

		if( index == 1 )
			return y;

		return z;
	}

	float x;
	float y;
	float z;
};

#endif // !__Math_Vector3D_h__
