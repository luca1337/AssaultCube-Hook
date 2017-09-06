#include "stdafx.h"
#include <Windows.h>

#pragma region PATTERN_MASKS
////////////////////////////////////////////////PATTERN AND MASKS////////////////////////////////////

//Armor and health
char* c_ArmorHealthPattern = "\x8B\x44\x24\x2C\x89\x8A\x00\x00\x00\x00\x89\x82\x00\x00\x00\x00\x0F\x94\xC1\x0F\xB6\xC1\x6A\x00\x50\x6A\xFF\x56\x57\x8B\xC2\xE8\x00\x00\x00\x00";
char* c_ArmorHealthMask = "xxxxxx????xx????xxxxxxxxxxxxxxxx????";

//LEt gun automatic shoot
char* c_AutomaticWeaponPattern = "\xFF\x46\x1C\x38\x9A\x28\x01\x00\x00\x75\x09\x8B\x46\x08\x88\x98\x24\x02\x00\x00\x8B\x4E\x04\x8B\x04\x8D\x00\x00\x00\x00";
char* c_AutomaticWeaponMask = "xxxxxxxxxxxxxxxxxxxxxxxxxx????";

//Automatic gun shoot restore
char* c_defaultWeaponPattern = "\x38\x9A\x28\x01\x00\x00\xEB\x09\x8B\x46\x08\x88\x98\x24\x02\x00\x00\x8B\x4E\x04";
char* c_defaultWeaponMask = "xxxxxxxxxxxxxxxxxxxx";

////////////////////////////////////////////////PATTERN AND MASKS////////////////////////////////////
#pragma endregion

#pragma region PADDING
//PADS
DWORD dwAEPad0 = 0xA;
DWORD dwAEPad1 = 0x4;
DWORD dwAWPad = 0x9;
DWORD dwRestorePad = 0x6;
#pragma endregion

//Local Player 
Reg *cReg = new Reg();

int a = 100;
int b = 150;
int c = 150;
int d = 150;

void MenuRender();

//OPCODES to patch memory
char NopHealthArmor[] = "\x90\x90\x90\x90\x90\x90";
char autoOpcodes[] = "\xEB\x09";
char autoDefOpcodes[] = "\x75\x09";

//general variables
bool bAuto = true;
bool bEnabled = false;

CMenu *g_Menu = nullptr;

char buffer[1024];

float screenX = GetSystemMetrics(SM_CXSCREEN);
float screenY = GetSystemMetrics(SM_CYSCREEN);


void OnHackSetup( )
{
	g_Menu = new CMenu( );

	g_Menu->AddHack( new CHack( "No Damage", false ) );
	g_Menu->AddHack( new CHack( "No Recoil", false ) );
	g_Menu->AddHack( new CHack( "No Scope", false ) );
	g_Menu->AddHack( new CHack( "Rapid Fire", false ) );
	g_Menu->AddHack( new CHack( "Automatic Guns", false ) );
	g_Menu->AddHack( new CHack( "Unlimited Ammo", false ) );
	g_Menu->AddHack( new CHack( "Fly Mode", false ) );
	g_Menu->AddHack( new CHack( "Player ESP", false ) );

	GameFunctions::AddHudLine( "Hack Loaded!" );
}

void OnHackSetup( CHack *pHack )
{
	//Disable fly mode once.
	if (!_stricmp(pHack->GetName( ), "Fly Mode") && !pHack->GetEnabled( ) && Game::GetInstance( ) && Game::GetInstance( )->m_LocalPlayer)
		Game::GetInstance()->m_LocalPlayer->m_SpectateMode = SM_NONE;

	//Patch Recoil
	if (!_stricmp(pHack->GetName( ), "No Recoil" ) )
		GameFunctions::PatchRecoil( pHack->GetEnabled( ) );

	//Patch Damage
	if (!_stricmp(pHack->GetName( ), "No Damage" ) )
		GameFunctions::PatchDamage( pHack->GetEnabled( ) );

	//Rapid Fire ? i don't know it seems rapid fire
	if (!_stricmp( pHack->GetName( ), "Rapid Fire" ) )
		GameFunctions::PatchRapidFire( pHack->GetEnabled( ) );

	//No scope im sure =)
	if(!_stricmp( pHack->GetName( ), "No Scope" ))
		GameFunctions::NoScope( pHack->GetEnabled( ) );

	//Auto guns + rapid fire. H4CK3D !
	if(!_stricmp( pHack->GetName( ), "Automatic Guns" ))
		GameFunctions::AutomaticGuns( pHack->GetEnabled( ) );

	//Unlimited ammo
	if(!_stricmp( pHack->GetName( ), "Unlimited Ammo" ) )
		GameFunctions::UnlimitedAmmo( pHack->GetEnabled( ) );
}

void MenuInput( )
{
	if ( HotKeys::Press( 0x28 ) )
		if ( g_Menu->GetSelectedIndex( ) < g_Menu->GetHacks( ).size( ) - 1 )
			g_Menu->SetSelectedIndex( g_Menu->GetSelectedIndex( ) + 1 );

	if ( HotKeys::Press( 0x26 ) )
		if (g_Menu->GetSelectedIndex() > 0)
			g_Menu->SetSelectedIndex(g_Menu->GetSelectedIndex() - 1);

	if ( HotKeys::Press( 0x25 ) )
	{
		CHack* pHack = g_Menu->GetHacks( ).at( g_Menu->GetSelectedIndex( ) );
		pHack->SetEnabled( false );
		OnHackSetup( pHack );
	}

	if ( HotKeys::Press( 0x27 ) )
	{
		CHack* pHack = g_Menu->GetHacks().at(g_Menu->GetSelectedIndex());
		pHack->SetEnabled(true);
		OnHackSetup(pHack);
	}
}

void DrawESP(Player *pTargetPlayer, Player *pLocalPlayer)
{
	Vec3 vHeadPos = pTargetPlayer->m_HeadPos;
	Vec3 vFootPos = pTargetPlayer->m_FootPos;

	vHeadPos.z += 0.8F; //Regular head pos is not high enough

	Vec3 vScreenHead, vScreenFoot;
	if (GameFunctions::WorldToScreen(vHeadPos, &vScreenHead) && GameFunctions::WorldToScreen(vFootPos, &vScreenFoot))
	{
		float flHeight = abs(vScreenFoot.y - vScreenHead.y);
		float flWidth = flHeight / 2.0F;

		RenderFunctions::Color32 color = GameFunctions::IsVisible(pLocalPlayer->m_HeadPos, pTargetPlayer->m_HeadPos) ? RenderFunctions::Color32(1.0F, 1.0F, 0.0F, 1.0F) : RenderFunctions::Color32(1.0F, 0.0F, 0.0F, 1.0F);

		DrawRect(vScreenHead.x - flWidth / 2, vScreenHead.y, vScreenHead.x + flWidth / 2, vScreenHead.y + flHeight, color, false);

		DrawHealthBar(vScreenHead.x - flWidth / 2, vScreenHead.y - 14, flWidth, 6.0F, static_cast < float >( static_cast < float >( pTargetPlayer->m_Health ) / 100.0F ), RenderFunctions::Color32(0.0F, 1.0F, 0.0F, 1.0F));

		GameFunctions::DrawString(vScreenFoot.x - flWidth / 2, vScreenFoot.y + 4, static_cast < int >( color.r * 255 ), static_cast < int >( color.g * 255 ), static_cast < int >( color.b * 255 ), pTargetPlayer->m_Name);
	}
}

void OnRenderFrame( )
{
	if ( HotKeys::Press( 0x70 ) ) //F1
		g_Menu->SetHackEnabled ( !g_Menu->GetIsHackEnabled( ) );

	if ( g_Menu->GetIsHackEnabled( ) ) //if menu is enabled run code
	{
		MenuInput();
		MenuRender();
	}

	Game *pGame = Game::GetInstance( );

	Player *pLocalPlayer = pGame->m_LocalPlayer;

	Weapon *pLocalWeapon = pLocalPlayer->GetCurrentWeapon();
	//Game hacks here 
	//init pointer to Game
	//get localplayer etc.. from here on
	//..
	//..
	//..

	if( g_Menu->GetHack( "Fly Mode" )->GetEnabled(  ) )
		pLocalPlayer->m_SpectateMode = SM_FLY;

	if (GetAsyncKeyState( VK_F4 ) & 1)
	{
		Weapon::Info *pInfo = pLocalWeapon->m_Info;

		pInfo->m_ReloadTime = 0.0f;
		pInfo->m_Recoil = 0.0f;
		pInfo->m_IsAuto = true;
		pInfo->m_RecoilIncrease = 0.0f;
		pInfo->m_MdlKickBack = 0.0f;
		pInfo->m_ProjSpeed = 1000.0f;
	}

	if (g_Menu->GetHack("Player ESP")->GetEnabled())
	{
		for (int i = 0; i < pGame->m_PlayerCount; i++)
		{
			Player* pPlayer = pGame->GetPlayer(i);
			if (!Utilss::IsValidPtr(pPlayer))
				continue;

			if (pPlayer == pLocalPlayer)
				continue;

			if (pPlayer->m_State != CS_ALIVE)
				continue;

			if (pPlayer->m_Health <= 0 || pPlayer->m_Health > 100)
				continue;

			if (pPlayer->m_HeadPos.x == 0 && pPlayer->m_HeadPos.y == 0 && pPlayer->m_HeadPos.z == 0)
				continue;

			if (GameFunctions::GotTeamMates())
			{
				if (pLocalPlayer->m_Team == pPlayer->m_Team) //No Teammates
					continue;
			}

			DrawESP(pPlayer, pLocalPlayer);
		}
	}
}

void MenuRender()
{
	glPushMatrix();

	float flScale = 0.8F;

	glScalef(flScale, flScale, 1.0F);

	int MENUX = 20;
	int MENUY = 180;
	int MENUYOFFSET = 20;
	int MENUOPTOFFSET = 200;

#pragma region HELPSTUFF
	//just to display values to change them shit
	/*sprintf_s(buffer, "x: %d y: %d x1: %d y1: %d", a, b, c, d);
	GameFunctions::DrawString( 400, 300, 0, 255, 255, buffer );

	if (HotKeys::Press(VK_F2))
		a++;
	if (HotKeys::Press(VK_F3))
		b++;
	if (HotKeys::Press(VK_F4))
		c++;
	if (HotKeys::Press(VK_F5))
		d++;

	if (HotKeys::Press(VK_F6))
		a--;
	if (HotKeys::Press(VK_F7))
		b--;
	if (HotKeys::Press(VK_F8))
		c--;
	if (HotKeys::Press(VK_F9))
		d--;*/
#pragma endregion

	/*POINT p;

	GetCursorPos(&p);

	sprintf_s(buffer, "x: %.2f y: %.2f", screenX, screenY);

	GameFunctions::DrawString( p.x, p.y, 255, 255, 0, buffer);*/
	
	DrawRect(316, 350, 17, 146, RenderFunctions::Color32(1, 0, 0, 0.35F), true);
	GameFunctions::DrawString(MENUX, MENUY - 30, 255, 255, 255, "nb37's ACube xHookx v 1.0");

	int i = 0;
	for (auto item = g_Menu->GetHacks().begin(); item != g_Menu->GetHacks().end(); ++item)
	{
		CHack *pHack = *item._Ptr; //item->second;

		if (i == g_Menu->GetSelectedIndex())
		{
			GameFunctions::DrawString(MENUX, MENUY + MENUYOFFSET * i, 255, 0, 0, pHack->GetName());
			GameFunctions::DrawString(MENUX + MENUOPTOFFSET, MENUY + MENUYOFFSET * i, 255, 50, 50, pHack->GetEnabled() ? "On" : "Off");
		}
		else
		{
			GameFunctions::DrawString(MENUX, MENUY + MENUYOFFSET * i, 255, 255, 255, pHack->GetName());
			GameFunctions::DrawString(MENUX + MENUOPTOFFSET, MENUY + MENUYOFFSET * i, 150, 150, 150, pHack->GetEnabled() ? "On" : "Off");
		}

		++i;
	}

	glScalef(1 / flScale, 1 / flScale, 1.0F);

	glPopMatrix();
}

__declspec ( naked ) void ShellCode() //per qualche motivo l'asm inline non va fatto cosi..
{
	__asm
	{
		/*mov esi, 0x14
	    inc dword ptr ds : [esi]
		push edi
		xor al, al
		mov eax, [ecx]
		xor al, cl
		jmp[0x67B5F3]*/
	}
}

#ifndef OLD_STUFF
#pragma region OLD STUFF
//NOP HEALTH
/*DWORD dwHealthPattern = Utils::FindPattern("ac_client.exe", c_ArmorHealthPattern, c_ArmorHealthMask);
DWORD dwArmorPattern = Utils::FindPattern("ac_client.exe", c_ArmorHealthPattern, c_ArmorHealthMask);

dwHealthPattern += dwAEPad0;
dwArmorPattern += dwAEPad1;

Utils::WPM(GetCurrentProcess(), (LPVOID)dwHealthPattern, NopHealthArmor, 6, NULL);
Utils::WPM(GetCurrentProcess(), (LPVOID)dwArmorPattern, NopHealthArmor, 6, NULL);*/

//Save value to regedit it seems to work
/*cReg->SaveToRegistry("Software\\ACubeHack\\Addresses\\Health", "Health", dwHealthPattern);
cReg->SaveToRegistry("Software\\ACubeHack\\Addresses\\Armor", "Armor", dwArmorPattern);*/
#pragma endregion
#pragma region OLD STUFF
//bAuto = !bAuto; // enable/disable gun automatic pistol, sniper rifle etc..

//if (bAuto) {
//	DWORD dwRestoreGunpattern = Utilss::FindPattern("ac_client.exe", c_defaultWeaponPattern, c_defaultWeaponMask);
//	dwRestoreGunpattern += dwRestorePad;
//	Utilss::WPM(GetCurrentProcess(), (LPVOID)dwRestoreGunpattern, autoDefOpcodes, 2, NULL);
//}
//else {
//	DWORD dwAutoGunPattern = Utilss::FindPattern("ac_client.exe", c_AutomaticWeaponPattern, c_AutomaticWeaponMask);
//	dwAutoGunPattern += dwAWPad;
//	Utilss::WPM(GetCurrentProcess(), (LPVOID)dwAutoGunPattern, autoOpcodes, 2, NULL);
//}
#pragma endregion
#endif // !OLD_STUFF


