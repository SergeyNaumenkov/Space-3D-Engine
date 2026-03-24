/*

    Copyright 2024 Sergey Naumenkov

    File: XmlScript.h
    Description: Xml parser via tinyXML2

    Date: 14/09/2024

*/

#ifndef __ScriptSystem_XmlScript_h__
#define __ScriptSystem_XmlScript_h__
#pragma once

#include "IXMLScript.h"

#include "Core/Vector.h"

// tinyXML2:
#include "tinyxml2.h"

struct XmlDocument_t
{
    tinyxml2::XMLDocument* mXmlDocument;
    const char* mDocumentName;
};

class XmlScript : public IXmlScript
{
public:
	XmlScript( );

    // Load script
    virtual bool LoadScript( const char* scriptPath, const char* scriptSaveName ) override;

    // Bind current document
    virtual void BindDocumentByName( const char* name ) override;

    // Get value from block
    virtual StringConverter GetNodeValue( const char* node, const char* child ) override;

private:
    Vector<XmlDocument_t> mXMLDocuments;
    tinyxml2::XMLDocument* mCurrentXMLDocument;
};

#endif // !__ScriptSystem_XmlScript_h__
