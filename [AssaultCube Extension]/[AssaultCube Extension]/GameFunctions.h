#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include "Classes.h"
#include "Utils.h"

using namespace AssaultCube;

namespace GameFunctions
{
	void EngineDrawString(char *pText, int x, int y, int r, int g, int b, int pUnknown = -1, int pUnknown2 = -1);

	bool IsVisible(Vec3 vFrom, Vec3 vTo);

	void DrawString(int x, int y, int r, int g, int b, char *pText, ...);

	void AddHudLine(char* pText);

	void PatchDamage ( bool bDamage );

	void PatchRecoil ( bool bDamage );

	void PatchRapidFire(bool bRapidFire);
}

#endif //!GAMEFUNCTIONS_H