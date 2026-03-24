# 3D Game Engine based on DirectX 11_1. It is based on a modular system. 
## Description Of All modules:
> \* Launcher - Exe launcher for engine. Loads engine dll and call Initialize his.<br>
> \* Engine - Main of all modules in system. Loads another modules and initialize their.<br>
> \* InputSystem - Core of all input for system. Makes checks input data from devices, check pressed keys and etc.<br>
> \* FileSystem - Core for work on files, directories and etc.<br>
> \* ScriptSystem - Loads special script files and store values.<br>
> \* Math - Library which stores basic mathematic functions.<br>
> \* InterfaceFactory - Library for loading dll and get class instance.(Sorry, c++ is not support direct export classes without linking .lib :D )<br>
> \* Core - Module stores Debugging, String Tools and etc functions what must need for another modules.<br>
> \* ShaderSystemDx11 - Shader System for DirectX 11_1 rendering system. Loads shaders from file and store their.<br>
> \* RenderSystemDx11 - Main module for rendering in all system. Right now, only Dx11 is support, maybe later I added new modules for rendering :D.<br>
> \* MaterialSystem - Module for loading and compiling textures from file or memory.<br>
> \* ShadersSourceCode - Library for storing source code of .hlsl shaders.<br>
> \* GameClient - Client Of Game.<br>
> \* SpaceLevelEditor - Map Editor for games.<br>
> \* sGui - Space Graphics User Interface.<br>
> \* sGuiControls - List of gui controls ( like a button,slider, panel etc ).<br>
## What is a modular system for?
Everything is simple, so that in the future it will be easy to replace any of the parts of the modular system.And most importantly, to create an SDK version, with a modular system, it is always easiest to remove closed systems and provide the user with open ones.
## Compile
> \* Visual Studio 2022<br>
> \* C++ support<br>
> \* DirectX 11_1 support by Your system<br>
## Third-Party
I try not to use libraries that cannot be distributed if they appear in the system (and they will) third-party libraries (they will be free for distribution), then I will leave them in the repository so that after cloning the repository you will not need anything
## Completion
Love your job and then everything will work out. Have Fun.❤️<br>
