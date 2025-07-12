#pragma once

namespace Game {

	class Events final : public REX::Singleton<Events>,
                         public RE::BSTEventSink<RE::MenuOpenCloseEvent> {

	public:
		static void Register();

	private:
		virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;
	};
}
