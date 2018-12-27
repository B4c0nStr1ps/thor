#ifndef THOR_WINDOWS_WINDOW_H_
#define THOR_WINDOWS_WINDOW_H_

#include "platform.h"
#include "windows_platform_api.h"
 
namespace Thor
{
	namespace Platform
	{
		class WindowsWindow
		{

		public:

			static std::shared_ptr<WindowsWindow> Make();

			static POINT CenterWindow(int32 width, int32 height);

			void Init(HINSTANCE ownerAppInstance, const ANSICHAR* className, const ANSICHAR* title, int32 desiredWidth, int32 desiredHeight);

			void Show();

			WindowsWindow();

		//private:


		private:

			HINSTANCE m_ownerAppInstance;

			HWND m_hwnd;

			int32 m_width;

			int32 m_height;

		};
	}
}
 
#endif // THOR_WINDOWS_WINDOW_H_
