#pragma once
#pragma comment(lib,"d3d11.lib") 
#pragma comment(lib,"dxgi.lib") 
#pragma comment(lib,"d3dcompiler.lib") 

#include <Windows.h>
class Window
{
	public:
		//Constructor
		Window();
		//Initialize the Window
		bool init();
		bool broadcast();
		//Release the Window
		bool release();
		bool isRunning();

		RECT getClientWindowRect();
		void setHWND(HWND hwnd);

		//EVENTS
		virtual void onCreate() = 0;
		virtual void onUpdate() = 0;
		virtual void onDestroy();

		//Destructor
		~Window();
	protected:
		HWND m_hwnd;
		bool m_is_run;

};

