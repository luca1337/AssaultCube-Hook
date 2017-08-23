#pragma once

#ifndef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif // !_MSC_VER

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <ostream>
#include <iostream>
#include <Psapi.h>
#include <math.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <string>
#include <tlhelp32.h>
#include "..\GL\glut.h"
#include "RegKey.h"
#include "Utils.h"
#include "Classes.h"
#include "Offsets.h"
#include "Enums.h"
#include "Menu.h"
#include "GameFunctions.h"
#include "Signatures.h"
#include "Hooks.h"
#include "Hack.h"
#include <GL/GL.h>
#include "RenderFunctions.h"
#include "HotKeys.h"

#pragma comment (lib, "psapi.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLu32.lib")
#pragma comment (lib, "freeglut.lib")

using namespace std;
using namespace AssaultCube;

//CUSTOM TYPEDEFS
typedef int(__stdcall *MsgBox)(HWND, LPCSTR, LPCSTR, UINT);


