#pragma comment(lib,"d3d11.lib") 
#pragma comment(lib,"DirectXTexD.lib") 

#include "AppWindow.h"
#include "EngineTime.h"

int main()
{
	AppWindow::initialize();
	AppWindow* runningApp = (AppWindow*)AppWindow::getInstance();
	EngineTime::initialize();
	runningApp->createGraphicsWindow();

	while (runningApp->isRunning())
	{
		runningApp->broadcast();
	}

	return 0;
}