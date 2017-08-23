#ifndef UTILS_H
#define UTILS_H

#include "stdafx.h"

namespace Utilss{
	#define M_PI 3.141592653589793F // as a float

	bool IsValidPtr(void *pPointer);
	void *DetourFunction(void *pLocation, void *pDetour, DWORD dwLength);
	bool MemoryEdit(void *pMemory, void *pSource, DWORD dwLength);
	DWORD FindPointer(DWORD dwOffsetLvl, HANDLE hProcHandle, DWORD dwBaseAddr, DWORD dwOffsets[], BYTE pad);
	void WriteToMemory(DWORD addressToWrite, char* valueToWrite, int byteNum);
	MODULEINFO GetModuleInfo(char *szModule);
	DWORD FindPattern(char *module, char *pattern, char *mask);
	BOOL WPM(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T lSize, SIZE_T* numOfBytesWritten);
};

#endif // !UTILS_H
