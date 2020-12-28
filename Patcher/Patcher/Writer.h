#pragma once

#include <windows.h>

#ifndef __wtypes_h__
#include <wtypes.h>
#endif
#include <iostream>
#ifndef __WINDEF_
#include <windef.h>
#endif

#include "atlstr.h"
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <cstring>

class Writer
{
public:
	Writer(int argc, TCHAR *argv[]);
	void Incert(int offset, char* str);
	~Writer();
private:
	#define BUFFERSIZE 6
	DWORD g_BytesTransferred = 0;

	VOID CALLBACK FileIOCompletionRoutine(
		__in  DWORD dwErrorCode,
		__in  DWORD dwNumberOfBytesTransfered,
		__in  LPOVERLAPPED lpOverlapped
	);

	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	char   ReadBuffer[BUFFERSIZE];
	OVERLAPPED ol;
	char DataBuffer[256];
};

