#include "stdafx.h"

Sigs *pSigs = new Sigs();

namespace GameFunctions
{
	void PatchRecoil(bool bNoRecoil)
	{
		if (bNoRecoil)
			Utilss::MemoryEdit(reinterpret_cast< void* >( OFFSET_PATCHRECOIL ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->RECOIL_OFF ) ), 10 );
		else
			Utilss::MemoryEdit(reinterpret_cast< void* >( OFFSET_PATCHRECOIL ), reinterpret_cast< BYTE* >( pSigs->GetSignature ( pSigs->RECOIL_ON ) ), 10 );
	}

	void PatchDamage(bool bNoDamage)
	{
		if (bNoDamage)
		{
			Utilss::MemoryEdit(reinterpret_cast< void* >( OFFSET_PATCHHEALTH ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->DAMAGE_HEALTH_OFF ) ), 6 );
			Utilss::MemoryEdit(reinterpret_cast< void* >( OFFSET_PATCHARMOR ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->DAMAGE_ARMOR_OFF ) ), 6 );
		}
		else
		{
			Utilss::MemoryEdit(reinterpret_cast< void* >( OFFSET_PATCHHEALTH ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->DAMAGE_HEALTH_ON ) ), 6 );
			Utilss::MemoryEdit(reinterpret_cast< void* >( OFFSET_PATCHARMOR ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->DAMAGE_ARMOR_ON ) ), 6 );
		}
	}

	void PatchRapidFire(bool bRapidFire)
	{
		if(bRapidFire)
			Utilss::MemoryEdit( reinterpret_cast< void * >( OFFSET_RAPIDFIRE ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->RAPID_FIRE_OFF ) ), 2 );
		else
			Utilss::MemoryEdit( reinterpret_cast< void * >( OFFSET_RAPIDFIRE ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->RAPID_FIRE_ON ) ), 2 );
	}

	void AddHudLine(char* pText)
	{
		using thudoutf = void(__cdecl *)(char* pText);
		thudoutf ohudoutf = reinterpret_cast< thudoutf >(OFFSET_HUDOUTF);

		ohudoutf(pText);
	}

	void EngineDrawString(char *pText, int x, int y, int r, int g, int b, int pUnknown, int pUnknown2)
	{
		__asm
		{
			push pUnknown2;
			push pUnknown;
			push b;
			push r;
			push y;
			push x;
			push pText;
			mov eax, g;
			mov ecx, OFFSET_ENGINE_DRAWTEXT;
			call ecx;
			add esp, 0x1C;
		}
	}

	bool IsVisible(Vec3 vFrom, Vec3 vTo)
	{
		__asm
		{
			push vTo.z;
			push vTo.y;
			push vTo.x;

			push vFrom.z;
			push vFrom.y;
			push vFrom.x;

			xor cl, cl;   //Tracer
			xor eax, eax;
			mov ebx, OFFSET_ISVISIBLE;
			call ebx;
			add esp, 0x18;
		}
	}

	void DrawString(int x, int y, int r, int g, int b, char *pText, ...)
	{
		va_list va_alist;

		char buf[256];

		va_start(va_alist, pText);
		_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
		va_end(va_alist);

		float extraScale = 0.3F;

		glScalef(extraScale, extraScale, 1);

		EngineDrawString(buf, x / extraScale, y / extraScale, r, g, b);

		glScalef(1 / extraScale, 1 / extraScale, 1);
	}
}

