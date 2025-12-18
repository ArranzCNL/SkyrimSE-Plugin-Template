#include "hook/MainHook.h"

#include "hook/FunctionHook.h"
#include "hook/InputHook.h"

#include "util/LogUtil.h"

namespace HOOK::MAIN {

	void Install()
	{
		LOG_INFO("Hooks initializing...");
		// Setup Variables
		HOOK::VARIABLE::fDeltaWorldTime = reinterpret_cast<const float*>(RELOCATION_ID(523660, 410199).address());

		HOOK::INPUT::Install();
	}
}
