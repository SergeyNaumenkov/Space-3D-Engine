/*

    Copyright 2024 Sergey Naumenkov

    File: IXmlScript.h
    Description: Public interface for xml script

    Date: 14/09/2024

*/

#ifndef __ScriptSystem_IXmlScript_h__
#define __ScriptSystem_IXmlScript_h__
#pragma once

#include "Core\StringConverter.h"

class IXmlScript
{
public:

    // Load script
    virtual bool LoadScript( const char* scriptPath, const char* scriptSaveName ) = 0;

    // Bind current document
    virtual void BindDocumentByName( const char* name ) = 0;

    // Get value from block
    virtual StringConverter GetNodeValue( const char* node, const char* child ) = 0;

private:

};

#endif // !__ScriptSystem_IXmlScript_h__
