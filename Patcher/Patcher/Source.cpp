#include <windows.h>

#ifndef __wtypes_h__
#include <wtypes.h>
#endif
#include <iostream>
#ifndef __WINDEF_
#include <windef.h>
#endif
#include "Writer.h"
int main() {
	int argc = 1;
	TCHAR argv[256];
	_tcscpy_s(argv, "a.txt");
	try {
		Writer a = Writer(argc, (TCHAR*)argv);
		char str[] = TEXT("nag nag nag");
		a.Incert(6, str);
	}
	catch (char* ex) {
		printf(ex);
	}
	
	system("pause");
}