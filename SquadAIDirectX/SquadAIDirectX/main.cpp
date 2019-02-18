#include "SystemManger.h"

 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
	SystemManger systemMan(hInstance);

	if (!systemMan.Init())
	{
		return 1;
	}
	return systemMan.Run();
}
