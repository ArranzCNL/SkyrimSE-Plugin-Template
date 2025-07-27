#pragma once

#include "RE/C/ControlMap.h"

namespace PROXY {

	struct ControlMap {

		static auto* Data(RE::ControlMap* a_controlMap)
		{
#if defined(SKYRIM_SUPPORT_NG)
			return &a_controlMap->GetRuntimeData();
#else
			return a_controlMap;
#endif
		}
	};
}
