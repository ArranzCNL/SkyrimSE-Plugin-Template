#pragma once

#include "RE/A/Actor.h"

namespace PROXY {

	struct Actor {

		static auto* Data(RE::Actor* a_actor)
		{
#if defined(SKYRIM_SUPPORT_NG)
			return &a_actor->GetActorRuntimeData();
#else
			return a_actor;
#endif
		}

		static auto* State(RE::Actor* a_actor)
		{
#if defined(SKYRIM_SUPPORT_NG)
			return a_actor->AsActorState();
#else
			return a_actor;
#endif
		}
	};
}
