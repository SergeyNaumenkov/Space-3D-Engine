/*

    Copyright 2024 Sergey Naumenkov

    File: XmlScript.cpp
    Description: Xml parser via tinyXML2

    Date: 14/09/2024

*/

#include "XmlScript.h"

#include "InterfaceFactory\InterfaceFactory.h"
#include "Core/Debug.h"
#include "Core\StringTools.h"

/*
    Constructor
*/
XmlScript::XmlScript( )
{

}

/*
    Load script
*/
bool XmlScript::LoadScript( const char* scriptPath, const char* scriptSaveName )
{
    tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument( );

    auto err = xmlDoc->LoadFile( scriptPath );
    if( err != tinyxml2::XML_SUCCESS )
    {
        PrintError( "Failed load xml script(%s)!", scriptPath );
        return false;
    }

    XmlDocument_t doc = { };
    doc.mXmlDocument = xmlDoc;
    doc.mDocumentName = scriptSaveName;
    mXMLDocuments.AddToBack( doc );

    return true;
}

/*
    Bind current document
*/
void XmlScript::BindDocumentByName( const char* name )
{
    for( int i = 0; i < mXMLDocuments.size(); i++ )
    {
        if( CompareStrings( name, mXMLDocuments[ i ].mDocumentName ) )
        {
            mCurrentXMLDocument = mXMLDocuments[ i ].mXmlDocument;
            break;
        }
    }
}

/*
    Get value from block
*/
StringConverter XmlScript::GetNodeValue( const char* node, const char* child )
{
    auto root = mCurrentXMLDocument->RootElement( );
    auto value = root->FirstChildElement( node )->FirstChildElement( child )->GetText( );
    return StringConverter( value );
}
