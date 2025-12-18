#include "event/GameEventManager.h"

#include "util/LogUtil.h"

namespace GAME_EVENT {

	void Manager::Register()
	{
		auto gameEvent = Manager::GetSingleton();
		RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(gameEvent);
	}

	RE::BSEventNotifyControl Manager::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto& menuName = a_event->menuName;
		LOG_INFO("[Menu]: {} ({})", menuName.c_str(), a_event->opening ? "Opening" : "Closing");

		return RE::BSEventNotifyControl::kContinue;
	}
}
