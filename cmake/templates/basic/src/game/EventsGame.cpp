#include "game/EventsGame.h"

#include "util/LogUtil.h"

namespace Game {

	void Events::Register()
	{
		auto gameEvent = Events::GetSingleton();
		RE::UI::GetSingleton()->AddEventSink<RE::MenuOpenCloseEvent>(gameEvent);
	}

	RE::BSEventNotifyControl Events::ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*)
	{
		if (!a_event) {
			return RE::BSEventNotifyControl::kContinue;
		}

		auto& menuName = a_event->menuName;
		LOG_INFO("[Menu]: {} ({})", menuName.c_str(), a_event->opening ? "Opening" : "Closing");

		return RE::BSEventNotifyControl::kContinue;
	}
}
