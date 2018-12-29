#include "thor/engine.h"
#include "thor/core/platform/platform_application.h"
#include "thor/core/diagnostics.h"

int32 Thor::ThorMain(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char *, int32 nCmdShow)
{
	THOR_TRACE(Diagnostics::TraceVerbosity::Information, "Test logging %s", "My param");
	auto app = Thor::Platform::Application::CreateApplication();
	app->Initialize();
	app->BeginLoop();
	return 0;
}
