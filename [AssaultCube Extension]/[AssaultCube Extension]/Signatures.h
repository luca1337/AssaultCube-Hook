#ifndef SIGNATURES_H
#define SIGNATURES_H

class Sigs
{
public:
	enum eSigs
	{
		//RECOIL
		RECOIL_OFF = 0,
		RECOIL_ON = 1,

		//DAMAGE
		DAMAGE_HEALTH_OFF = 2,
		DAMAGE_ARMOR_OFF = 3,
		DAMAGE_HEALTH_ON = 4,
		DAMAGE_ARMOR_ON = 5,

		//RAPID FIRE
		RAPID_FIRE_OFF = 6,
		RAPID_FIRE_ON = 7,

		//NO SCOPE
		NO_SCOPE_OFF = 8,
		NO_SCOPE_ON = 9,

		//AUTOMATIC GUNS
		AUTOGUNS_ON = 10,
		AUTOGUNS_OFF = 11,

		//AMMO
		UNLIMITEDAMMO_ON = 12,
		UNLIMITEDAMMO_OFF = 13,
	};

	char *GetSignature(eSigs eIndex)
	{
		return this->m_Sigs[eIndex];
	}

private:
	char *m_Sigs[1024] =
	{
		"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90",		//No recoil patched
		"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2",		//recoil normal

		"\x90\x90\x90\x90\x90\x90",						//NOP for health
		"\x90\x90\x90\x90\x90\x90",						//NOP for armor
		"\x89\x82\xF8\x00\x00\x00",						//health
		"\x89\x8A\xFC\x00\x00\x00",						//armor

		"\x90\x90",										// rapid fire off + 4 bytes ? test
		"\x89\x0A",										// rapid fire restored + ( \x8B\x76\x14 ) ?? 4 bytes more ?

		"\x90\x90\x90",									//nop for no scope
		"\x88\x50\x28",									//no scope restored

		"\xEB\x09",										//jmp for auto guns
		"\x75\x09",										//restore auto guns

		"\x90\x90",										//nop for ulmited ammo
		"\xFF\x0E",										//restore unl ammo
	};
};

#endif // !SIGNATURES_H
