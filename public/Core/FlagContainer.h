/*

    Copyright 2024 Sergey Naumenkov

    File: FlagContainer.h
    Description: Flag container with fast search

    Date: 01/09/2024

*/

#ifndef __Core_FlagContainer_h__
#define __Core_FlagContainer_h__
#pragma once

#include <iostream>

template<class T>
class FlagContainer
{
    static_assert( std::is_integral<T>::value || std::is_enum<T>::value,
                   "FlagContainer can only be used with integral or enum types." );

public:
	FlagContainer( );

    // Add flag
    void AddFlag( T flag );

    // Try find flag in collection
    bool HasFlag( T flag );

    // Remove all flags
    void RemoveAll( );

    // Remove one flag
    void RemoveFlag( T flag );

private:
    unsigned int mFlags;
};

/*
    Constructor
*/
template<class T>
inline FlagContainer<T>::FlagContainer( )
{
    mFlags = 0;
}

/*
    Add flag
*/
template<class T>
inline void FlagContainer<T>::AddFlag( T flag )
{
    auto val = static_cast< unsigned int >( flag );
    mFlags |= val;
}

/*
    Try find flag in collection
*/
template<class T>
inline bool FlagContainer<T>::HasFlag( T flag )
{
    auto val = static_cast< unsigned int >( flag );
    return ( mFlags & val ) != 0;
}

/*
    Remove all flags
*/
template<class T>
inline void FlagContainer<T>::RemoveAll( )
{
    mFlags = 0;
}

/*
    Remove one flag
*/
template<class T>
inline void FlagContainer<T>::RemoveFlag( T flag )
{
    auto val = static_cast< unsigned int >( flag );
    mFlags &= ~val;
}


#endif // !__Core_FlagContainer_h__