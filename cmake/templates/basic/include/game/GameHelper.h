#pragma once

#include "plugin.h"
#include "hook/FunctionHook.h"

#include "util/LogUtil.h"

namespace GAME::HELPER {

	static inline const float& Get_DeltaWorldTime()
	{
		return *HOOK::VARIABLE::fDeltaWorldTime;
	}

	static inline void GetCurrentAnimations(bool a_firstPerson = false)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		if (!player) {
			return;
		}
		// For demostration purpose
		auto consoleLog = RE::ConsoleLog::GetSingleton();
		if (!consoleLog) {
			return;
		}

		RE::BSAnimationGraphManagerPtr graphManager;
		player->GetAnimationGraphManager(graphManager);
		if (!graphManager) {
			return;
		}

		RE::BShkbAnimationGraphPtr project = graphManager->graphs[a_firstPerson];
		auto behaviourGraph = project ? project->behaviorGraph : nullptr;
		if (!behaviourGraph) {
			return;
		}

		auto activeNodes = behaviourGraph->activeNodes;
		if (!activeNodes) {
			return;
		}

		RE::hkbClipGenerator* clipGenerator;
		// For demostration purpose
		consoleLog->Print("[Behavior Project] %s", project->projectName.c_str());

		for (auto nodeInfo : *activeNodes) {

			if (!nodeInfo.nodeClone) {
				continue;
			}

			clipGenerator = skyrim_cast<RE::hkbClipGenerator*>(nodeInfo.nodeClone);
			if (!clipGenerator) {
				continue;
			}
			// For demostration purpose
			consoleLog->Print("[Animation] %s", clipGenerator->animationName.c_str());
		}
		return;
	}

#if defined(_DEBUG)
	static inline void StackTrace(bool a_external = false)
	{
		void* stack[64]{ nullptr };
		std::uint16_t frames = RtlCaptureStackBackTrace(0, 64, stack, nullptr);

		auto pluginInfo = &DLLMAIN::Plugin::GetSingleton()->Info();
		auto gameBaseAddress = pluginInfo->gameBaseAddress;
		auto gameImageSize = pluginInfo->gameImageSize;

		for (std::uint16_t i = 0; i < frames; ++i) {

			auto addr = reinterpret_cast<std::uintptr_t>(stack[i]);
			if (addr >= gameBaseAddress && addr <= (gameBaseAddress + gameImageSize)) {
				auto offset = (addr - gameBaseAddress) - 1;
				LOG_DEBUG("SkyrimSE Frame[{}]: 0x{:X}", i, offset);
			}
			else if (a_external) {
				LOG_DEBUG("External Frame[{}]: 0x{:X}", i, addr);
			}
		}
	}
#endif
}
