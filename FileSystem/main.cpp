/*

	Copyright 2024 Sergey Naumenkov

	File: main.cpp
	Description: File System

	Date: 02/08/2024

*/

#include "IFileSystem.h"

#include "InterfaceFactory/InterfaceFactory.h"
#include "Core/StringTools.h"
#include "Core/Debug.h"

class FileSystem : public IFileSystem
{
public:
	FileSystem( );
	~FileSystem( );

	// Pre initialize module
	virtual bool PreInit( ) override;

	// Initialize module
	virtual bool Initialize( ) override;

	// Start 
	virtual void Start( ) { };

	// Shutdown module
	virtual void Shutdown( ) override;

	// Set new directory
	virtual bool SetDirectory( const char* newDir ) override;

	// Get current directory
	virtual const char* GetDirectory( ) override;

	// Set File open mode
	virtual void SetOpenMode( FileMode_t mode ) override;

	// Get Current file offset
	virtual long GetFileOffset( ) override;

	// Set file offset
	virtual void SetFileOffset( long offset ) override;

	// Check file
	virtual bool IsFileExist( const char* file ) override;

	// Open and read file
	virtual const char* OpenAndReadFile( const char* file, long* fileSize = 0 ) override;

	// Open/create file for write
	virtual bool OpenWriteFile( const char* file ) override;

	// Write buffer into file
	virtual void WriteToFile( void* data, long bufferSize ) override;

	// Get file size
	virtual long GetFileSize( const char* file, bool checkExistFile = false ) override;
	virtual long GetFileSize( void* filePtr ) override;

	// Get file extension
	virtual const char* GetFileExtension( const char* file ) override;

	// Get File Name
	virtual const char* GetFileName( const char* file, bool savePath = false ) override;

	// Close current file
	virtual void CloseCurrentFile( ) override;

private:
	const char* mCurrentDirectory;

	FILE* mCurrentFile;
	FileMode_t mCurrentFileMode;
};

/*
	Constructor
*/
FileSystem::FileSystem( )
{
	mCurrentDirectory = "";

	mCurrentFileMode = FileMode_t::File_None;
	mCurrentFile = nullptr;
}

/*
	Destructor
*/
FileSystem::~FileSystem( )
{
	mCurrentFile = nullptr;
	mCurrentDirectory = nullptr;
}

bool FileSystem::PreInit( )
{
	return true;
}

/*
	Initialize module
*/
bool FileSystem::Initialize( )
{
	return true;
}

/*
	Shutdown module
*/
void FileSystem::Shutdown( )
{

}

/*
	Set base directory
*/
bool FileSystem::SetDirectory( const char* newDir )
{
	char curDir[ MAX_PATH ];
	auto ret = GetModuleFileName( 0, curDir, MAX_PATH );
	char* dir = new char[ MAX_PATH ];

	if( ret != 0 )
	{
		auto exeName = strrchr( curDir, '\\' );

		// Remove '\\'
		exeName++;
		int exeLen = ( int )strlen( exeName );
		int lenToCopy = ( int )strlen( curDir ) - ( int )strlen( "bin\\" ) - exeLen;

		strncpy_s( dir, lenToCopy + 1, curDir, lenToCopy );
		dir[ lenToCopy + 1 ] = 0;

		SetCurrentDirectory( dir );
		mCurrentDirectory = dir;
	}
	else
	{
		return false;
	}

	return true;
}

/*
	Get Directory
*/
const char* FileSystem::GetDirectory( )
{
	return mCurrentDirectory;
}

/*
	Set File open mode
*/
void FileSystem::SetOpenMode( FileMode_t mode )
{
	mCurrentFileMode = mode;
}

/*
	Get Current file offset
*/
long FileSystem::GetFileOffset( )
{
	if( !mCurrentFile )
	{
		PrintError( "File is not open!" );
		return -1;
	}

	return ftell( mCurrentFile );
}

/*
	Set file offset
*/
void FileSystem::SetFileOffset( long offset )
{
	if( !mCurrentFile )
	{
		PrintError( "File is not open!" );
		return;
	}

	fseek( mCurrentFile, offset, 0 );
}

/*
	Check file
*/
bool FileSystem::IsFileExist( const char* file )
{
	FILE* f = nullptr;
	auto ret = fopen_s( &f, file, "rb" );

	if( ret == 0 )
	{
		fclose( f );
		return true;
	}
	else
	{
		return false;
	}
}

/*
	Convert FileMode_t to C File Mode (const char*)
*/
const char* GetFileMode( FileMode_t mode )
{
	switch( mode )
	{
		case FileMode_t::File_WriteBinary:
			return "wb";
		case FileMode_t::File_Write:
			return "a";
		case FileMode_t::File_Open:
			return "r";
		case FileMode_t::File_OpenBinary:
			return "rb";
			break;
		default:
			return "r";
			break;
	}
}

/*
	Open and read file
*/
const char* FileSystem::OpenAndReadFile( const char* file, long* fileSize )
{
	FILE* f = nullptr;
	auto ret = fopen_s( &f, file, GetFileMode( mCurrentFileMode ) );
	if( ret != 0 )
	{
		return nullptr;
	}

	// Get file size
	auto fSize = GetFileSize( f );

	// Copy file size
	if( fileSize )
	{
		*fileSize = fSize;
	}

	// Create buffer
	char* dataBuffer = new char[ fSize + 1 ];

	auto copied = fread_s( dataBuffer, fSize + 1, 1, fSize, f );
	fclose( f );

	// Reset open mode
	mCurrentFileMode = FileMode_t::File_None;

	return CopyString( dataBuffer, static_cast< long >( copied ) );
}

/*
	Open/create file for write
*/
bool FileSystem::OpenWriteFile( const char* file )
{
	// Open or Create file
	FILE* f = nullptr;
	auto ret = fopen_s( &f, file, GetFileMode( mCurrentFileMode ) );

	if( ret != 0 )
	{
		return false;
	}

	mCurrentFile = f;

	return true;
}

/*
	Write buffer into file
*/
void FileSystem::WriteToFile( void* data, long bufferSize )
{
	fwrite( data, bufferSize, 1, mCurrentFile );
}

/*
	Get file size
*/
long FileSystem::GetFileSize( const char* file, bool checkExistFile )
{
	if( checkExistFile )
	{
		if( !IsFileExist( file ) )
		{
			return -1;
		}
	}

	FILE* f = nullptr;
	auto ret = fopen_s( &f, file, "rb" );
	if( !f )
		return -1;

	// Get size
	fseek( f, 0L, SEEK_END );
	auto size = ftell( f );
	fseek( f, 0L, SEEK_SET );

	fclose( f );

	return size;
}

/*
	Get file size
*/
long FileSystem::GetFileSize( void* filePtr )
{
	FILE* f = reinterpret_cast< FILE* >( filePtr );
	if( !f )
		return -1;

	// Get size
	fseek( f, 0L, SEEK_END );
	auto size = ftell( f );
	fseek( f, 0L, SEEK_SET );

	return size;
}

/*
	Get file extension
*/
const char* FileSystem::GetFileExtension( const char* file )
{
	auto filePathLength = strlen( file );
	char* extension = nullptr;
	int cursorPos = 0;

	// Copy path
	auto path = file;

	for( int i = 0; i < filePathLength; i++ )
	{
		if( path[ 0 ] == '.' )
		{
			path++;

			// Create string
			auto l = strlen( path );
			extension = new char[ l + 1 ];

			// Start parsing
			for( int i = 0; i < l; i++ )
			{
				extension[ cursorPos++ ] = path[ 0 ];
				path++;
			}

			// All done
			break;
		}

		// Next char
		path++;
	}

	if( extension )
	{
		extension[ cursorPos ] = '\0';
	}

	return extension;
}

/*
	Get File Name
*/
const char* FileSystem::GetFileName( const char* file, bool savePath )
{
	// Fix shashes
	file = FixSlashesInString( file );

	// Fix it code
	const char* str = nullptr;
	str = strrchr( file, '\\' );

	if( str )
	{
		str++;
	}
	else
	{
		str = file;
	}

	// Get len without path
	int len = StringLength( savePath == true ? file : str );

	// Get file extension length
	auto extLen = StringLength( GetFileExtension( savePath == true ? file : str ) );

	// Copy without extension
	// extLen - 1 it's '.' char
	str = CopyString( savePath == true ? file : str, len - extLen - 1 );

	return str;
}

/*
	Close current file
*/
void FileSystem::CloseCurrentFile( )
{
	if( mCurrentFile )
		fclose( mCurrentFile );
}

SaveDllClass( FileSystem, IFileSystemV1 )