#ifndef THOR_ENGINE_H_
#define THOR_ENGINE_H_

#include "core/platform/platform.h"
#include "core/platform/windows_platform_api.h"

namespace Thor
{
	int32 ThorMain(_In_ HINSTANCE hInInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ char*, _In_ int32 nCmdShow);
}
 
#endif // THOR_ENGINE_H_
