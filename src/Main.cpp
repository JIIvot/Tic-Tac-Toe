#include "Game.h"

#if defined( NDEBUG ) && defined( _WINDOWS )
#include <Windows.h>

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	CGame::Start();
	return 0;
}
#else
int main()
{
	CGame::Start();
	return 0;
}
#endif
