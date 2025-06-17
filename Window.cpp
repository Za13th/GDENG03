#include "Window.h"
#include "EngineTime.h"
#include "imgui.h"

extern IMGUI_IMPL_API LRESULT CALLBACK ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM uparam, LPARAM lparam);

Window::Window()
{
	this->m_is_run = false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM uparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, uparam, lparam))
		return true; // If ImGui is handling the message, return true to prevent further processing


	switch (msg)
	{
		case WM_CREATE:
		{
			// Event fired when the window is created
			// collected here..
			Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
			// .. and then stored for later lookup
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
			window->setHWND(hwnd);
			window->onCreate();
			break;
		}
		case WM_SETFOCUS:
		{
			// Event fired when the window gets focused (whatever the hell that means)
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onFocus();
			break;
		}
		case WM_KILLFOCUS:
		{
			// Event fired when the window gets focused (whatever the hell that means)
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onKillFocus();
			break;
		}
		case WM_DESTROY:
		{
			// Event fired when the window is destroyed
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->onDestroy();
			::PostQuitMessage(0);
			break;
		}
		default:
			return ::DefWindowProc(hwnd, msg, uparam, lparam);
	}
	return NULL;
}

bool Window::init()
{
	//setting up stuff
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadCursor(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadCursor(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;
	
	if (!::RegisterClassEx(&wc)) //if class registration fails, return false
		return false;
	//create window here
	this->m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MyWindowClass", L"DirectX Application", 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
		CW_USEDEFAULT, 1024, 768,
		NULL, NULL, NULL, this);

	if (!this->m_hwnd) //if window creation failed, return false
		return false;

	::ShowWindow(this->m_hwnd, SW_SHOW);
	::UpdateWindow(this->m_hwnd);

	this->m_is_run = true;
	return true;
}

bool Window::broadcast()
{

	EngineTime::LogFrameStart();

	this->onUpdate();
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	EngineTime::LogFrameEnd();

	return true;
}
bool Window::release()
{
	//DESTROY THE WINDOW
	if (::DestroyWindow(this->m_hwnd))
		return false;

	return true;
}

bool Window::isRunning()
{
	return this->m_is_run;
}

RECT Window::getClientWindowRect()
{
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);

	return rc;
}

void Window::setHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

void Window::onCreate()
{

}

void Window::onUpdate()
{

}

void Window::onDestroy()
{
	this->m_is_run = false;
}

void Window::onFocus()
{

}

void Window::onKillFocus()
{

}
Window::~Window()
{

}