#include "MemeReader.h"
#include <tlhelp32.h>


MemeReader::MemeReader(char* procName, size_t bufferSize)
{
	int len = strlen(procName) + 1;
	m_pName = new char[len];
	strcpy_s(m_pName, len, procName);
	m_buffer = new byte[bufferSize];
}

//Choose process from all process list

MemeReader::MemeReader(size_t bufferSize)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(snapshot, &pInfo)) {
		while (Process32Next(snapshot, &pInfo)) {
			std::wcout << "Name: " << pInfo.szExeFile << " Size:" << pInfo.dwSize
				<< " Threads " << pInfo.cntThreads << "\n";
		}
	}
	char procName[32];
	try {
		std::cout << "Type process name >>";
		std::cin >> procName;
		int len = strlen(procName) + 1;
		m_pName = new char[len];
		strcpy_s(m_pName, len, procName);

		m_buffer = new byte[bufferSize];
	}
	catch(_exception e){
		std::cout << e.name;
	}
}

MemeReader::~MemeReader(void)
{
	delete[] m_pName;
	delete[] m_buffer;
}

void MemeReader::GetPID() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(snapshot, &pInfo)) {
		while (Process32Next(snapshot, &pInfo)) {
			std::cout << m_pName << (char*)pInfo.szExeFile << strcmp(m_pName, (char*)pInfo.szExeFile) 
				<< "\n";
			if (strcmp(m_pName, (char*)pInfo.szExeFile) == 0) {
				m_pID = pInfo.th32ParentProcessID;
				CloseHandle(snapshot);
				return;
			}
		}
	}
	CloseHandle(snapshot);
	m_pID = 0;
	return;
}

void MemeReader::Open(DWORD acessRights) {
	GetPID();
	m_hProc = OpenProcess(acessRights, false, m_pID);
}
void MemeReader::Close() {
	CloseHandle(m_hProc);
}
DWORD MemeReader::getPID() {
	return m_pID;
}
//Write to opened process memory


