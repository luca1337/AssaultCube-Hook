#include "stdafx.h"

void Reg::SaveToRegistry(LPCSTR lpSection, LPCSTR lpValueName, DWORD dwValue)
{
	DWORD dwResult;
	HKEY hKey;

	dwResult = RegOpenKeyEx(HKEY_CURRENT_USER, lpSection, 0, KEY_WRITE, &hKey);

	if (dwResult != ERROR_SUCCESS)
	{
		DWORD dwDisposition;

		dwResult = RegCreateKeyEx(HKEY_CURRENT_USER, lpSection, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);

		RegSetValueEx(hKey, lpValueName, 0, REG_DWORD, reinterpret_cast<BYTE*>(&dwValue), sizeof(dwValue));

		RegCloseKey(hKey);
	}
}

void Reg::ReadFromRegistry(LPCSTR lpValueName, DWORD dwValue)
{
	DWORD dwResult;
	HKEY hKey;

	dwResult = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\ACubeHack", 0, KEY_WRITE, &hKey);

	/*if (dwResult != ERROR_SUCCESS)
	{
		DWORD dwDisposition;

		dwResult = RegQueryInfoKeyA()
	}*/
}