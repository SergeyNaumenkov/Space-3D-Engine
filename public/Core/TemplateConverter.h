/*

    Copyright 2024 Sergey Naumenkov

    File: TemplateConverter.h
    Description: Template functions for converters

    Date: 14/09/2024

*/

#ifndef __Core_TemplateConverter_h__
#define __Core_TemplateConverter_h__
#pragma once

/*
    Convert value to new type
*/
template<class T> T ConvertToNewValue( void* value )
{
    return reinterpret_cast< T >( value );
}

#endif // !__Core_TemplateConverter_h__
