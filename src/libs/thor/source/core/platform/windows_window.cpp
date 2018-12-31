#include "thor/core/platform/windows_window.h"


Thor::SharedRef<Thor::Platform::WindowsWindow> Thor::Platform::WindowsWindow::Make()
{
	auto window = Thor::MakeShared<WindowsWindow>();
	return window;
}

POINT Thor::Platform::WindowsWindow::CenterWindow(int32 width, int32 height)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	POINT center;
	center.x = (screenWidth - width) / 2;
	center.y = (screenHeight - height) / 2;
	return center;
}

void Thor::Platform::WindowsWindow::Init(HINSTANCE ownerAppInstance, const ANSICHAR* className, const ANSICHAR* title, int32 desiredWidth, int32 desiredHeight)
{	
	m_width = desiredWidth;
	m_height = desiredHeight;
	//define window style .
	//docs https://docs.microsoft.com/en-us/windows/desktop/winmsg/extended-window-styles
	uint32 windowStyle = 0;
	uint32 windowExStyle = 0;
	windowStyle = WS_OVERLAPPEDWINDOW;
	windowExStyle = WS_EX_APPWINDOW;

	RECT borderRect = { 0,0,0,0 };
	AdjustWindowRectEx(&borderRect, windowStyle, false, windowExStyle);

	POINT center = CenterWindow(desiredWidth, desiredHeight);

	// Inflate the window size by the OS border
	int32 windowWidth = desiredWidth + borderRect.right - borderRect.left;
	int32 windowHeight = desiredHeight + borderRect.bottom - borderRect.top;

	m_hwnd = CreateWindowEx(windowExStyle, className, title, windowStyle, center.x, center.y, windowWidth, windowHeight, nullptr, nullptr, m_ownerAppInstance, nullptr);
}

void Thor::Platform::WindowsWindow::Show()
{
	ShowWindow(m_hwnd, SW_SHOWNORMAL);
}

Thor::Platform::WindowsWindow::WindowsWindow()
	:m_ownerAppInstance(nullptr), m_hwnd(nullptr), m_width(0), m_height(0)
{
}
