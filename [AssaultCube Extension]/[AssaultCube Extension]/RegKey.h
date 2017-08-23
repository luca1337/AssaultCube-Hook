#pragma once
#include "stdafx.h"

class Reg
{
public:
	void SaveToRegistry(LPCSTR lpSection, LPCSTR lpValueName, DWORD dwValue);
	void ReadFromRegistry(LPCSTR lpValueName, DWORD dwValue);
};