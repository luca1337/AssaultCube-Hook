#include "stdafx.h"
#include "Hack.h"

extern int main();

MsgBox Msg = NULL;

DWORD WINAPI MainThread(LPVOID lpParameter)
{
	//Hook an ingame function so we won't get conflicts with differnt opengl.dll versions, I used to hook wglSwapBuffers before

	OnHackSetup( );

	//main();

	dwDrawHudJmpBack = reinterpret_cast< DWORD >(Utilss::DetourFunction(reinterpret_cast< void* >(OFFSET_GL_DRAWHUD_MIDFUNC), gl_DrawHud, 13));

	return EXIT_SUCCESS;
}

BOOL WINAPI DllMain(HMODULE _hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		HMODULE hModule = ::LoadLibrary("User32.dll");
		CreateThread(0, 0, MainThread, 0, 0, 0);
		Msg = reinterpret_cast<MsgBox>(::GetProcAddress(hModule, "MessageBoxA"));
		(*Msg)(NULL, "Attached", "Message!", MB_OK);
		break;
	}

	return TRUE;
}