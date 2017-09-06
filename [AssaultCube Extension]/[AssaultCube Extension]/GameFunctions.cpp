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
			Utilss::MemoryEdit( reinterpret_cast< void* >( OFFSET_RAPIDFIRE ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->RAPID_FIRE_OFF ) ), 2 );
		else
			Utilss::MemoryEdit( reinterpret_cast< void* >( OFFSET_RAPIDFIRE ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->RAPID_FIRE_ON ) ), 2 );
	}

	void NoScope( bool bNoScope )
	{
		if( bNoScope )
			Utilss::MemoryEdit( reinterpret_cast< void* > ( OFFSET_NOSCOPE ), reinterpret_cast< BYTE* > ( pSigs->GetSignature( pSigs->NO_SCOPE_OFF ) ), 3 );
		else
			Utilss::MemoryEdit( reinterpret_cast< void* > ( OFFSET_NOSCOPE ), reinterpret_cast< BYTE* > ( pSigs->GetSignature( pSigs->NO_SCOPE_ON ) ), 3 );
	}

	void AutomaticGuns( bool bAutomatic )
	{
		if ( bAutomatic )
			Utilss::MemoryEdit( reinterpret_cast< void* >( OFFSET_AUTOMATICGUN ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->AUTOGUNS_ON ) ), 2 );
		else
			Utilss::MemoryEdit( reinterpret_cast< void* >( OFFSET_AUTOMATICGUN ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->AUTOGUNS_OFF ) ), 2 );
	}

	void UnlimitedAmmo( bool bUnlimitedAmmo )
	{
		if (bUnlimitedAmmo)
			Utilss::MemoryEdit( reinterpret_cast< void* >( OFFSET_AMMUNITIONS ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->UNLIMITEDAMMO_ON ) ), 2 );
		else
			Utilss::MemoryEdit( reinterpret_cast< void* >( OFFSET_AMMUNITIONS ), reinterpret_cast< BYTE* >( pSigs->GetSignature( pSigs->UNLIMITEDAMMO_OFF) ), 2 );
	}

	void AddHudLine(char* pText)
	{
		using thudoutf = void(__cdecl *)(char* pText);
		thudoutf ohudoutf = reinterpret_cast< thudoutf >(OFFSET_HUDOUTF);

		ohudoutf(pText);
	}

	bool GotTeamMates()
	{
		DWORD dwGameMode = *reinterpret_cast< DWORD* >(OFFSET_GAMEMODE);
		return (
			dwGameMode == GMODE_BOTTEAMONESHOTONKILL ||
			dwGameMode == GMODE_TEAMONESHOTONEKILL ||
			dwGameMode == GMODE_BOTTEAMDEATHMATCH ||
			dwGameMode == GMODE_TEAMDEATHMATCH ||
			dwGameMode == GMODE_TEAMSURVIVOR ||
			dwGameMode == GMODE_TEAMLSS ||
			dwGameMode == GMODE_CTF ||
			dwGameMode == GMODE_TEAMKEEPTHEFLAG ||
			dwGameMode == GMODE_HUNTTHEFLAG ||
			dwGameMode == GMODE_TEAMPF ||
			dwGameMode == GMODE_BOTTEAMSURVIVOR ||
			dwGameMode == GMODE_BOTTEAMONESHOTONKILL
			);
	}

	bool WorldToScreen(Vec3 vPos, Vec3 *pvOut)
	{
		ScreenSettings *pScreenSettings = ScreenSettings::GetInstance();
		if (!Utilss::IsValidPtr(pScreenSettings))
			return false;

		glmatrixf *mvpmatrix = reinterpret_cast< glmatrixf* >(OFFSET_MVPMATRIX);

		float mX = (float)pScreenSettings->m_Width / 2.0F;
		float mY = (float)pScreenSettings->m_Height / 2.0F;

		float x = mvpmatrix->transformx(vPos);
		float y = mvpmatrix->transformy(vPos);
		float z = mvpmatrix->transformz(vPos);
		float w = mvpmatrix->transformw(vPos);

		if (w < 0.01F)
			return false;

		pvOut->x = mX + (mX * x / w);
		pvOut->y = mY - (mY * y / w);
		pvOut->z = w;

		return true;
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

