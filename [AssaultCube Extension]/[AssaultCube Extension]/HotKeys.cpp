#include "stdafx.h"

SHORT HotKeys::Press( int vKey )
{
	if (GetAsyncKeyState( vKey ) & 1 )
		return true;
	return false;
}
