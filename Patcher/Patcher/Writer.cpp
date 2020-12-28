#include "Writer.h"
Writer::Writer(int argc, TCHAR *argv[]) {
	if (argc != 2)
	{
		throw "Incorrect arg count";
	}
	if (argv != NULL) {
		printf(argv[1]);
		hFile = CreateFile(argv[1],
			GENERIC_ALL,
			0,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	}
	if (hFile == INVALID_HANDLE_VALUE)
	{
		throw "Error opening process: Invalid process handle";
	}
}

Writer::~Writer() {
	CloseHandle(hFile);
}

void Writer::Incert(int offset, char* str) {
	ol.Offset = offset;
	strcpy_s(DataBuffer, str);
	BOOL bErrorFlag = FALSE;
	bErrorFlag = WriteFile(
		hFile,
		DataBuffer,
		strlen(DataBuffer),
		NULL,
		&ol);
}