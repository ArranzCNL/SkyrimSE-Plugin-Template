#pragma once

#include "RE/P/PlayerCharacter.h"

namespace PROXY {

	struct PlayerCharacter {

		static auto* ActorData(RE::PlayerCharacter* a_player)
		{
#if defined(SKYRIM_SUPPORT_NG)
			return &a_player->GetActorRuntimeData();
#else
			return a_player;
#endif
		}

		static auto* ActorState(RE::PlayerCharacter* a_player)
		{
#if defined(SKYRIM_SUPPORT_NG)
			return a_player->AsActorState();
#else
			return a_player;
#endif
		}

		static auto* Data(RE::PlayerCharacter* a_player)
		{
#if defined(SKYRIM_SUPPORT_NG)
			return &a_player->GetPlayerRuntimeData();
#else
			return a_player;
#endif
		}

		static auto* MagicTarget(RE::PlayerCharacter* a_player)
		{
#if defined(SKYRIM_SUPPORT_NG)
			return a_player->AsMagicTarget();
#else
			return a_player;
#endif
		}
	};
}
