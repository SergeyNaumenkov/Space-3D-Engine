/*

    Copyright 2024 Sergey Naumenkov

    File: ScriptSystem.cpp
    Description: Script System

    Date: 02/08/2024

*/

#include "IScriptSystem.h"
#include "XmlScript.h"

#include "InterfaceFactory/InterfaceFactory.h"

class ScriptSystem : public IScriptSystem
{
public:
	ScriptSystem( );
	~ScriptSystem( );

    // Get xml script
    virtual IXmlScript* GetXmlScript( ) override;

private:
    XmlScript* mXmlScript;
};

/*
    Constructor
*/
ScriptSystem::ScriptSystem( )
{
    mXmlScript = nullptr;
}

/*
    Destructor
*/
ScriptSystem::~ScriptSystem( )
{

}

/*
    Get xml script
*/
IXmlScript* ScriptSystem::GetXmlScript( )
{
    if( !mXmlScript )
    {
        mXmlScript = new XmlScript( );
    }

    return mXmlScript;
}

SaveDllClass( ScriptSystem, IScriptSystemV1 )