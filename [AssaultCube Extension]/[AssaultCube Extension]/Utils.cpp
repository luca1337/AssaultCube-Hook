#include "stdafx.h"

namespace Utilss
{
	bool IsValidPtr(void *pPointer)
	{
		if (pPointer && HIWORD(pPointer))
		{
			if (!IsBadReadPtr(pPointer, sizeof(DWORD_PTR)))
				return true;
		}
		return false;
	}

	void *DetourFunction(void *pLocation, void *pDetour, DWORD dwLength)
	{
		void *pTrampoline = VirtualAlloc( nullptr, dwLength + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		memcpy(pTrampoline, pLocation, dwLength);

		*reinterpret_cast< BYTE* >(reinterpret_cast< DWORD >(pTrampoline) + dwLength) = 0xE9;
		*reinterpret_cast< DWORD* >(reinterpret_cast< DWORD >(pTrampoline) + dwLength + 1) = reinterpret_cast< DWORD >(pLocation) - (reinterpret_cast< DWORD >(pTrampoline) + dwLength);

		DWORD dwOldProtection = NULL;
		VirtualProtect(pLocation, dwLength, PAGE_EXECUTE_READWRITE, &dwOldProtection);

		*reinterpret_cast< BYTE* >(reinterpret_cast< DWORD >(pLocation)) = 0xE9;
		*reinterpret_cast< DWORD* >(reinterpret_cast< DWORD >(pLocation) + 1) = reinterpret_cast< DWORD >(pDetour) - (reinterpret_cast< DWORD >(pLocation) + 5);

		for (int i = 5; i < dwLength; i++)
			*reinterpret_cast< BYTE* >(reinterpret_cast< DWORD >(pLocation) + i) = 0x90;

		VirtualProtect(pLocation, dwLength, dwOldProtection, NULL);

		return pTrampoline;
	}

	bool MemoryEdit(void *pMemory, void *pSource, DWORD dwLength)
	{
		DWORD dwOldProtect = NULL;
		if (VirtualProtect(pMemory, dwLength, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		{
			for (int i = 0; i < dwLength; i++)
				*reinterpret_cast< BYTE* >(reinterpret_cast< DWORD >(pMemory) + i) = *reinterpret_cast< BYTE* >(reinterpret_cast< DWORD >(pSource) + i);

			VirtualProtect(pMemory, dwLength, dwOldProtect, 0);
			return true;
		}
		return false;
	}

	MODULEINFO GetModuleInfo(char *szModule)
	{
		MODULEINFO modinfo = { 0 };
		HMODULE hModule = GetModuleHandle(szModule);
		if (hModule == 0)
			return modinfo;
		GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
		return modinfo;
	}

	DWORD FindPattern(char *module, char *pattern, char *mask)
	{
		MODULEINFO mInfo = GetModuleInfo(module);
		DWORD base = reinterpret_cast < DWORD >( mInfo.lpBaseOfDll );
		DWORD size = static_cast < DWORD >( mInfo.SizeOfImage );
		DWORD patternLength = static_cast < DWORD >( strlen( mask ) );

		for (DWORD i = 0; i < size - patternLength; i++)
		{
			bool found = true;
			for (DWORD j = 0; j < patternLength; j++)
			{
				found &= mask[j] == '?' || pattern[j] == *reinterpret_cast < char* >( base + i + j );
			}

			if (found)
			{
				return base + i;
			}
		}

		return NULL;
	}

	DWORD FindPointer(DWORD dwOffsetLvl, HANDLE hProcHandle, DWORD dwBaseAddr, DWORD dwOffsets[], BYTE pad)
	{
		DWORD dwBase = dwBaseAddr;
		DWORD dwLevels = dwOffsetLvl;

		for (DWORD i = 0; i < dwLevels; i++)
		{
			ReadProcessMemory(hProcHandle, reinterpret_cast < LPVOID >( dwBase ), &dwBase, pad, nullptr);
			dwBase += dwOffsets[i];
		}
		return dwBase;
	}

	void WriteToMemory(DWORD addressToWrite, char* valueToWrite, int byteNum)
	{
		unsigned long OldProtection;
		VirtualProtect(reinterpret_cast < LPVOID >( addressToWrite ), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);

		memcpy(reinterpret_cast < LPVOID >( addressToWrite ), valueToWrite, byteNum);

		VirtualProtect(reinterpret_cast < LPVOID >( addressToWrite ), byteNum, OldProtection, NULL);
	}

	BOOL WPM(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T lSize, SIZE_T* numOfBytesWritten)
	{
		return WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, lSize, numOfBytesWritten);
	}
}