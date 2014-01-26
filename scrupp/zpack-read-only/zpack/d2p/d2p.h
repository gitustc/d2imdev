#ifndef __D2P_H__
#define __D2P_H__

#include <string.h>
#include <stdint.h>

typedef struct {
    uint32_t    len;
}D2PKGHDR;


typedef struct {
    D2PKGHDR    hdr;
}D2PKG;


/* namespace zp */
/* { */
/* #if defined (ZP_USE_WCHAR) */
/* 	typedef wchar_t Char; */
/* 	#ifndef _T */
/* 		#define _T(str) L##str */
/* 	#endif */
/* 	typedef std::wstring String; */
/* 	#define Fopen _wfopen */
/* 	#define Strcpy wcscpy_s */
/* #else */
/* 	typedef char Char; */
/* 	#ifndef _T */
/* 		#define _T(str) str */
/* 	#endif */
/* 	typedef std::string String; */
/* 	#define Fopen fopen */
/* 	#define Strcpy strcpy_s */
/* #endif */
/*  */
/* typedef unsigned char u8; */
/* typedef unsigned short u16; */
/* typedef unsigned long uint32_t; */
/* typedef unsigned long long u64; */

const uint32_t MAX_FILENAME_LEN = 1024;

const uint32_t OPEN_READONLY = 1;
const uint32_t OPEN_NO_FILENAME = 2;

const uint32_t PACK_UNICODE = 1;

const uint32_t FILE_DELETE = (1<<0);
const uint32_t FILE_COMPRESS = (1<<1);
//const uint32_t FILE_WRITING = (1<<2);

const uint32_t FILE_FLAG_USER0 = (1<<10);
const uint32_t FILE_FLAG_USER1 = (1<<11);

typedef bool (*Callback)(const Char* path, zp::uint32_t fileSize, void* param);

class IReadFile;
class IWriteFile;

///////////////////////////////////////////////////////////////////////////////////////////////////
class IPackage
{
public:
	virtual bool readonly() const = 0;

	virtual const Char* packageFilename() const = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//readonly functions, not available when package is dirty
	//IFile will become unavailable after package is modified

	virtual bool hasFile(const Char* filename) const = 0;
	virtual IReadFile* openFile(const Char* filename) = 0;
	virtual void closeFile(IReadFile* file) = 0;

	virtual uint32_t getFileCount() const = 0;
	virtual bool getFileInfo(uint32_t index, Char* filenameBuffer, uint32_t filenameBufferSize, uint32_t* fileSize = 0,
							uint32_t* packSize = 0, uint32_t* flag = 0, uint32_t* availableSize = 0, u64* contentHash = 0) const = 0;
	virtual bool getFileInfo(const Char* filename, uint32_t* fileSize = 0, uint32_t* packSize = 0,
							uint32_t* flag = 0, uint32_t* availableSize = 0, u64* contentHash = 0) const = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//package manipulation fuctions, not available in read only mode

	//do not add same file more than once between flush() call
	//outFileSize	origin file size
	//outPackSize	size in package
	virtual bool addFile(const Char* filename, const Char* externalFilename, uint32_t fileSize, uint32_t flag,
						uint32_t* outPackSize = 0, uint32_t* outFlag = 0, uint32_t chunkSize = 0) = 0;

	virtual IWriteFile* createFile(const Char* filename, uint32_t fileSize, uint32_t packSize,
									uint32_t chunkSize = 0, uint32_t flag = 0, u64 contentHash = 0) = 0;
	virtual IWriteFile* openFileToWrite(const Char* filename) = 0;
	virtual void closeFile(IWriteFile* file) = 0;

	//can not remove files added after last flush() call
	virtual bool removeFile(const Char* filename) = 0;

	//return true if there's any unsaved change of package
	virtual bool dirty() const = 0;

	//package file won't change before calling this function
	virtual void flush() = 0;

	virtual bool defrag(Callback callback, void* callbackParam) = 0;	//can be very slow, don't call this all the time

	virtual uint32_t getFileUserDataSize() const = 0;

	virtual bool writeFileUserData(const Char* filename, const u8* data, uint32_t dataLen) = 0;
	virtual bool readFileUserData(const Char* filename, u8* data, uint32_t dataLen) = 0;

protected:
	virtual ~IPackage(){}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class IReadFile
{
public:
	virtual uint32_t size() const = 0;

	virtual uint32_t availableSize() const = 0;

	virtual uint32_t flag() const = 0;

	virtual void seek(uint32_t pos) = 0;

	virtual uint32_t tell() const = 0;

	virtual uint32_t read(u8* buffer, uint32_t size) = 0;

protected:
	virtual ~IReadFile(){}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
class IWriteFile
{
public:
	virtual uint32_t size() const = 0;

	virtual uint32_t flag() const = 0;

	virtual void seek(uint32_t pos) = 0;

	virtual uint32_t tell() const = 0;

	virtual uint32_t write(const u8* buffer, uint32_t size) = 0;

protected:
	virtual ~IWriteFile(){}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
IPackage* create(const Char* filename, uint32_t chunkSize = 0x40000, uint32_t fileUserDataSize = 0);
IPackage* open(const Char* filename, uint32_t flag = OPEN_READONLY | OPEN_NO_FILENAME);
void close(IPackage* package);

}
