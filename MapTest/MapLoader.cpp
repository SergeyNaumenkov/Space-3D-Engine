#include <windows.h>

#include "BaseRenderer\IBaseRenderer.h"
#include "InterfaceFactory\InterfaceFactory.h"

typedef IBaseRenderer* ( *GetBaseRendererFunc )( const char* renderName );

int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
   
    return 0;
}
