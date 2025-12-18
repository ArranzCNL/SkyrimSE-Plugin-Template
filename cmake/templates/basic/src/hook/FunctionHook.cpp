#include "hook/FunctionHook.h"

namespace HOOK {

	namespace FUNCTION {

		void FadeOutGame(bool a_fadingOut, bool a_blackFade, float a_fadeTransitionDuration, bool a_holdFade, float a_fadeHoldDuration)
		{
			using func_t = decltype(&FadeOutGame);
			static REL::Relocation<func_t> func{ RELOCATION_ID(51909, 52847) };
			return func(a_fadingOut, a_blackFade, a_fadeTransitionDuration, a_holdFade, a_fadeHoldDuration);
		}
	}

	namespace VARIABLE {
		const float* fDeltaWorldTime;
	}
}
