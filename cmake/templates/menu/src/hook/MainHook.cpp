#include "hook/MainHook.h"

#include "hook/InputHook.h"
#include "hook/GraphicsHook.h"
#include "util/LogUtil.h"

namespace Hook {

	namespace Main {

		void Install()
		{
			LOG_INFO("Hooks initializing...");

			Graphics::Install();
			Input::Install();
		}
	}
}
