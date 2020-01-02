// DD_Tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

#define FILE_NAME_BUF_SIZE  (32768)
#define ARHIVE_FILE         (_T("\\Documents\\Projects\\DD Data\\DD\\SG.DL1"))
#define MAX_ARCH_NAME_SIZE  (12)
#define CVT_BUF_SIZE        (0x200ul)

#pragma pack(push, 1)

struct ArhiveHdr{
	BYTE    bSign[6];
	WORD    wReserved1;
	WORD    wEntryCount;
	DWORD   dwFileTableOffset;
	WORD    wReserved2;
};

struct ArhiveFileEntry{
	char    pcFileName[MAX_ARCH_NAME_SIZE];
	DWORD   dwFileSize;
};

#pragma pack(pop)

char    g_pcCvtString[CVT_BUF_SIZE];
TCHAR   g_ptCvtString[CVT_BUF_SIZE];

int ConvertMBCSToUni(const char *cpcMBString, wchar_t *pwcUString, int iUStringSize)
{
	return MultiByteToWideChar(932 /* Shift-JIS */, 0, cpcMBString, -1, pwcUString, iUStringSize);
}

int ConvertUniToMBCS(const wchar_t *cpwUString, char *pcMBString, int iMBStringSize)
{
	return WideCharToMultiByte(932 /* Shift-JIS */, 0, cpwUString, -1, pcMBString, iMBStringSize, NULL, NULL);
}

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR   pcFileName[FILE_NAME_BUF_SIZE];
	size_t  sFileNameSize = 0;
	HANDLE  hFile;
	HANDLE  hFileMap;
	void    *pvBuffer;
	LARGE_INTEGER   ilFileSize;
	DWORD   dwOffset;
	DWORD   dwFileOffset;
	ArhiveHdr   sArhiveHeader;
	//vector<ArhiveFileEntry>   vArhiveFileEntry;
	ArhiveFileEntry sArhiveFileEntry;
	int iIdx;

	sFileNameSize = GetEnvironmentVariable(_T("USERPROFILE"), pcFileName, FILE_NAME_BUF_SIZE);
	if (sFileNameSize == 0) {
		_tprintf(_T("User profile not found\n"));
		return -1;
	}

	_tcscat_s(pcFileName, ARHIVE_FILE);

	hFile = CreateFile(pcFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE){
		_tprintf(_T("Can't open script file: %s\n"), pcFileName);
		return -2;
	}

	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hFileMap == NULL){
		_tprintf(_T("Couldn't create file mapping: %s"), pcFileName);
		CloseHandle(hFile);
		return -3;
	}

	pvBuffer = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
	if (pvBuffer == NULL){
		_tprintf(_T("Couldn't map view of file: %s"), pcFileName);
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		return -4;
	}

	GetFileSizeEx(hFile, &ilFileSize);

	memcpy(&sArhiveHeader, pvBuffer, sizeof(ArhiveHdr));

	if (sArhiveHeader.bSign[0] != 'D' || sArhiveHeader.bSign[1] != 'L' || sArhiveHeader.bSign[2] != '1'
		|| (sArhiveHeader.dwFileTableOffset + sArhiveHeader.wEntryCount * sizeof(ArhiveFileEntry)) > ilFileSize.LowPart){

		_tprintf(_T("This is not a valid file: %s"), pcFileName);
		UnmapViewOfFile(pvBuffer);
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		return -5;
	}

	dwOffset = sArhiveHeader.dwFileTableOffset;
	dwFileOffset = sizeof(ArhiveHdr);
	for (iIdx = 0; iIdx < sArhiveHeader.wEntryCount && dwOffset < ilFileSize.LowPart; iIdx++){
		memcpy(&sArhiveFileEntry, (BYTE*)pvBuffer + dwOffset, sizeof(ArhiveFileEntry));
		dwOffset += sizeof(ArhiveFileEntry);
		strncpy_s(g_pcCvtString, CVT_BUF_SIZE, sArhiveFileEntry.pcFileName, MAX_ARCH_NAME_SIZE);
		ConvertMBCSToUni(g_pcCvtString, g_ptCvtString, CVT_BUF_SIZE);
		_tprintf(_T("%06d\t%s\t%ud\t%ud\n"), iIdx, g_ptCvtString, sArhiveFileEntry.dwFileSize, dwFileOffset);
		dwFileOffset += sArhiveFileEntry.dwFileSize;
	}

	UnmapViewOfFile(pvBuffer);
	CloseHandle(hFileMap);
	CloseHandle(hFile);

	return 0;
}

