#pragma once

namespace GAME_EVENT {

	class Manager final : public REX::Singleton<Manager>,
                         public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

	public:
		static void Register();

	private:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
	};
}
