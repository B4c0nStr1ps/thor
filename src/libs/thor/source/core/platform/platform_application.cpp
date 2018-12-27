#include "thor/core/platform/platform_application.h"
#include "thor/core/platform/windows_window.h"

//declare global instance
std::shared_ptr<Thor::Platform::Application> MainApplication = nullptr;

const ANSICHAR Thor::Platform::Application::AppWindowClass[] = "ThorAppWindow";

std::shared_ptr<Thor::Platform::Application> Thor::Platform::Application::CreateApplication()
{	
	HINSTANCE ownerInstance = (HINSTANCE)GetModuleHandle(NULL);
	Application::RegisterWinClass(ownerInstance);
	MainApplication = std::make_shared<Application>();
	MainApplication->m_instanceHandle = ownerInstance;
	return MainApplication;
}

void Thor::Platform::Application::PumMessages()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Thor::Platform::Application::Initialize()
{
	m_mainWindow = WindowsWindow::Make();
	m_mainWindow->Init(m_instanceHandle, AppWindowClass, "Thor application", 1920, 1080);
	m_mainWindow->Show();
}

void Thor::Platform::Application::BeginLoop()
{
	while (!QuitRequested)
	{
		Application::PumMessages();
	}
}

Thor::Platform::Application::Application()
	: m_instanceHandle(nullptr), m_mainWindow(nullptr), QuitRequested(false)
{
}

bool Thor::Platform::Application::RegisterWinClass(const HINSTANCE owner)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = AppWndProc;
	wc.cbClsExtra = 0L;
	wc.cbWndExtra = 0L;
	wc.hInstance = owner;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = Application::AppWindowClass;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	return false;
}

LRESULT Thor::Platform::Application::AppWndProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
	case WM_QUIT:
		MainApplication->QuitRequested = true;
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
