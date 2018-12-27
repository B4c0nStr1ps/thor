#ifndef THOR_PLATFORM_APPLICATION_H_
#define THOR_PLATFORM_APPLICATION_H_
 
#include "platform.h"
#include "windows_platform_api.h"
#include "windows_window.h"

namespace Thor
{
	namespace Platform
	{
		class Application
		{

		public:

			static std::shared_ptr<Application> CreateApplication();

			static void PumMessages();

			void Initialize();

			void BeginLoop();

			Application();

			bool QuitRequested;

		public:

			static const ANSICHAR AppWindowClass[];

		private:


			static bool RegisterWinClass(const HINSTANCE owner);

			static LRESULT CALLBACK AppWndProc(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam);

		private:
			HINSTANCE m_instanceHandle;

			std::shared_ptr<WindowsWindow> m_mainWindow;
		};
	}
}
 
#endif // THOR_PLATFORM_APPLICATION_H_