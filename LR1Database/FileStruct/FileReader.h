#pragma once
#include <string.h>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <iomanip>
class FileReader
{
public:
	FileReader(const char* path = "");
	char Read(int offset);
	void Write(const char* buf, int offset);
	int ReadInt(int offset);
	void WriteInt(int offset, int _Int);
	void clear(int offset, int size) {
		for (int i = offset; i < offset + size; i++)
			Write(i);
	}
	int size();
	~FileReader();
private:								//for fileread
	void AppendFile(int offset);
	char* buf;
	void Write(int offset);
	void DisplayError(LPTSTR lpszFunction);
	HANDLE hFile;
	OVERLAPPED ol;
	void ToHexStr(int Num);
};

