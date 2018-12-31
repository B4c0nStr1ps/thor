#include "thor/engine.h"
#include "thor/core/platform/platform_application.h"
#include "thor/core/diagnostics.h"

#include "thor/core/array_list.h"

THOR_DEFINE_TRACE_LISTENER_STATIC(MainListener);

int32 Thor::ThorMain(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char *, int32 nCmdShow)
{
	Thor::ArrayList<int> myArray;
	myArray.Emplace(101);

	THOR_TRACE(MainListener, Diagnostics::TraceVerbosity::Information, "Test logging %s", "My param");
	auto app = Thor::Platform::Application::CreateApplication();
	app->Initialize();
	app->BeginLoop();
	return 0;
}
