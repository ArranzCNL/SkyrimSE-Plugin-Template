#include "hook/MainHook.h"

#include "hook/InputHook.h"
#include "hook/GraphicsHook.h"
#include "util/LogUtil.h"

namespace HOOK::MAIN {

	void Install()
	{
		LOG_INFO("Hooks initializing...");

		HOOK::Graphics::Install();
		HOOK::INPUT::Install();
	}
}
