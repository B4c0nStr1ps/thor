
#include <thor/core/platform/platform.h>
#include <thor/core/platform/windows_platform_api.h>
#include <thor/engine.h>

int32 WINAPI WinMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char* cmdLine, _In_ int32 nCmdShow)
{
	return Thor::ThorMain(hInInstance, hPrevInstance, cmdLine, nCmdShow);
}