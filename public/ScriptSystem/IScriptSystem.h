/*

    Copyright 2024 Sergey Naumenkov

    File: IScriptSystem.h
    Description: Public interface for load script

    Date: 02/08/2024

*/

#ifndef __ScriptSystem_IScriptSystem_h__
#define __ScriptSystem_IScriptSystem_h__
#pragma once

#include "IXMLScript.h"

class IScriptSystem
{
public:

    // Get xml script
    virtual IXmlScript* GetXmlScript( ) = 0;

};

#endif // !__ScriptSystem_IScriptSystem_h__
