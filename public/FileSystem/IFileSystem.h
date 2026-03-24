/*

    Copyright 2024 Sergey Naumenkov

    File: IFileSystem.h
    Description: Public Interface for File System

    Date: 02/08/2024

*/

#ifndef __FileSystem_IFileSystem_h__
#define __FileSystem_IFileSystem_h__
#pragma once

#include "ModuleLoader\IModule.h"

enum class FileMode_t
{
    File_None,
    File_WriteBinary,
    File_Write,
    File_Open,
    File_OpenBinary
};

class IFileSystem : public IModule
{
public:

    // Pre initialize module
    virtual bool PreInit( ) = 0;

    // Initialize module
    virtual bool Initialize( ) = 0;

    // Start 
    virtual void Start( ) = 0;

    // Shutdown module
    virtual void Shutdown( ) = 0;

    // Set new directory
    virtual bool SetDirectory( const char* newDir ) = 0;

    // Get current directory
    virtual const char* GetDirectory( ) = 0;

    // Set File open mode
    virtual void SetOpenMode( FileMode_t mode ) = 0;

    // Check file
    virtual bool IsFileExist( const char* file ) = 0;

    // Get Current file offset
    virtual long GetFileOffset( ) = 0;

    // Set file offset
    virtual void SetFileOffset( long offset ) = 0;

    // Open and read file
    virtual const char* OpenAndReadFile( const char* file, long* fileSize = 0 ) = 0;

    // Open/create file for write
    virtual bool OpenWriteFile( const char* file ) = 0;

    // Write buffer into file
    virtual void WriteToFile( void* data, long bufferSize ) = 0;

    // Get file size
    virtual long GetFileSize( const char* file, bool checkExistFile = false ) = 0;
    virtual long GetFileSize( void* filePtr ) = 0;

    // Get file extension
    virtual const char* GetFileExtension( const char* file ) = 0;

    // Get File Name
    virtual const char* GetFileName( const char* file, bool savePath = false ) = 0;

    // Close current file
    virtual void CloseCurrentFile( ) = 0;

};

#endif // !__FileSystem_IFileSystem_h__
