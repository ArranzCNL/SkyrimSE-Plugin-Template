#include "hook/MainHook.h"

#include "hook/FunctionHook.h"
#include "hook/InputHook.h"

#include "util/LogUtil.h"

namespace Hook {

	namespace Main {

		void Install()
		{
			LOG_INFO("Hooks initializing...");
			// Setup Variables
			Hook::Variable::fDeltaWorldTime = reinterpret_cast<const float*>(RELOCATION_ID(523660, 410199).address());

			Hook::Input::Install();
		}
	}
}
