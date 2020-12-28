#pragma once
#include <iostream>
#include "Windows.h"
class MemeReader
{
public:
	MemeReader(char* procName, size_t bufferSize = 32);
	MemeReader(size_t bufferSize = 32);
	~MemeReader(void);

	void Open(DWORD acessRights = PROCESS_ALL_ACCESS);
	void Close();
	DWORD getPID();
	

	//additional functions
private:
	void GetPID();
	char* m_pName;
	HANDLE m_hProc;
	DWORD m_pID;
	byte* m_buffer;
	size_t m_bufferSize;
};

