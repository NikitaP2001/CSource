#include "FileReader.h"
#define READ_BUF_SIZE 5
FileReader::FileReader(const char* path) {
    ol = { 0 };
    char _path[256] = "";
    strcpy_s(_path, strlen(path) + 1, path);
    hFile = CreateFileA(_path,                // name of the write
        GENERIC_READ | GENERIC_WRITE,          // open for writing
        0,                      // do not share
        NULL,                   // default security
        OPEN_EXISTING,             // create new file only
        FILE_FLAG_OVERLAPPED,  // normal file
        NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        hFile = CreateFileA(_path,              
            GENERIC_READ | GENERIC_WRITE,        
            0,                  
            NULL,                  
            CREATE_NEW,          
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,  
            NULL);
    }
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DisplayError(TEXT("WriteFile"));
        printf("Terminal failure: Unable to write to file.\n");
    }
};

FileReader::~FileReader() {
    CloseHandle(hFile);
};

void FileReader::Write(const char* buf, int offset) {
    ol.Offset = offset;
    bool bErrorFlag = WriteFileEx(
        hFile,           // open file handle
        buf,      // start of data to write
        strlen(buf) ,  // number of bytes to write
        &ol,
        0);            // no overlapped structure

    if (FALSE == bErrorFlag)
    {
        DisplayError(TEXT("WriteFile"));
        printf("Terminal failure: Unable to write to file.\n");
    }
}

void FileReader::Write(int offset) {

    BYTE buf[1] = { 0 };
    ol.Offset = offset;
    DWORD towrite = 1;
    bool bErrorFlag = WriteFileEx(
        hFile,           // open file handle
        buf,      // start of data to write
        towrite,  // number of bytes to write
        &ol,
        0);            // no overlapped structure

    if (FALSE == bErrorFlag)
    {
        DisplayError(TEXT("WriteFile"));
        printf("Terminal failure: Unable to write to file.\n");
    }
}

char FileReader::Read(int offset) {
    AppendFile(offset);
    ol.Offset = offset;
    char ReadBuffer[READ_BUF_SIZE] = { 0 };
    if (FALSE == ReadFileEx(hFile, ReadBuffer, READ_BUF_SIZE - 1, &ol, 0))
    {
        DisplayError(TEXT("ReadFile"));
        printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
        return NULL;
    }
    return ReadBuffer[0];
};

void FileReader::DisplayError(LPTSTR lpszFunction)
// Routine Description:
// Retrieve and output the system error message for the last-error code
{
    LPVOID lpMsgBuf = {0};
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0,
        NULL);

    lpDisplayBuf =
        (LPVOID)LocalAlloc(LMEM_ZEROINIT,
            (lstrlen((LPCTSTR)lpMsgBuf)
                + lstrlen((LPCTSTR)lpszFunction)
                + 40) // account for format string
            * sizeof(TCHAR));

    if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error code %d as follows:\n%s"),
        lpszFunction,
        dw,
        lpMsgBuf)))
    {
        printf("FATAL ERROR: Unable to output error code.\n");
    }

    _tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}

int FileReader::ReadInt(int offset) {
    unsigned char buf1[4] = { 0 };
    for (int i = 0; i <= 3; i++) {
        buf1[i] = Read(i + offset);
    }
    int res = ((buf1[3] << 24) + (buf1[2] << 16) + (buf1[1] << 8) + buf1[0]);
    return res;
}

void FileReader::WriteInt(int offset, int _Int) {
    clear(offset, sizeof(int));
    ToHexStr(_Int);
    std::string s = std::to_string(_Int);
    char const* pchar = s.c_str();
    for (int i = 0; i < strlen(buf) / 2; i++)
    {
        char temp = buf[i];
        buf[i] = buf[strlen(buf) - i - 1];
        buf[strlen(buf) - i - 1] = temp;
    }
    Write(buf, offset);
}

void FileReader::ToHexStr(int Num) {   //using template instead of int it might recieve any type
    buf = new char[sizeof(int)+1];
    memset(buf, 0, (sizeof(int) + 1));
    int i = 0;
    buf[i] = (Num >> 24) & 0xFF;
    if (((Num >> 24) & 0xFF) != 0) {
        i++;
    }
    buf[i] = (Num >> 16) & 0xFF;
    if (((Num >> 16) & 0xFF) != 0) {
        i++;
    }
    buf[i] = (Num >> 8) & 0xFF;
    if (((Num >> 8) & 0xFF) != 0) {
        i++;
    }
    buf[i] = Num & 0xFF;
}

void FileReader::AppendFile(int offset) {
    DWORD b = 0;
    b = GetFileSize(hFile, NULL);
    if (b < offset) {
        LARGE_INTEGER a = { 48 };
        SetFilePointerEx(hFile, a, NULL, FILE_END);
        SetEndOfFile(hFile);
    }
}

int FileReader::size() {
    DWORD b = 0;
    b = GetFileSize(hFile, NULL);
    return b;
}