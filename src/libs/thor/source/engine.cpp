#include "thor/engine.h"
#include "thor/core/platform/platform_application.h"

int32 Thor::ThorMain(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char *, int32 nCmdShow)
{
	auto app = Thor::Platform::Application::CreateApplication();
	app->Initialize();
	app->BeginLoop();
	return 0;
}
