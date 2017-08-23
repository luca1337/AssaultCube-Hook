#ifndef MENU_H
#define MENU_H

#include <Windows.h>
#include <vector>

class CHack
{
public:
	CHack(char *pName, bool pEnabled);

	inline char *GetName()
	{
		return this->m_Name;
	}

	inline bool GetEnabled() const
	{
		return this->m_Enabled;
	}


	inline void SetEnabled(bool bValue)
	{
		this->m_Enabled = bValue;
	}

	inline DWORD GetLastTick() const
	{
		return this->m_LastTick;
	}

	inline void SetLastTick(DWORD dwValue)
	{
		this->m_LastTick = dwValue;
	}

private:
	char m_Name[32];
	bool m_Enabled;
	DWORD m_LastTick;
};

class CMenu
{
public:

	CMenu();

	void AddHack(CHack *pHack);

	int GetHackCount() const;

	CHack *GetHack(char *pName) const;

	inline std::vector<CHack *> &GetHacks()
	{
		return this->m_Hacks;
	}

	inline int GetSelectedIndex() const
	{
		return this->m_SelectedIndex;
	}

	inline void SetSelectedIndex(int iValue)
	{
		this->m_SelectedIndex = iValue;
	}

	inline bool GetIsHackEnabled() const
	{
		return this->m_Enabled;
	}

	inline void SetHackEnabled(bool bValue)
	{
		this->m_Enabled = bValue;
	}

private:
	std::vector<CHack *> m_Hacks;
	int m_SelectedIndex;
	bool m_Enabled;
};

#endif // !MENU_H

