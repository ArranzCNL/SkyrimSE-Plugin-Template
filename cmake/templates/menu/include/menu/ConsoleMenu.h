#pragma once

#include "imgui/imgui.h"

namespace Menu {

	class Console final {

	public:
		Console();
		virtual ~Console() = default;

		void Update();
		void AddLog(std::string_view a_msg);

	private:
		void ClearLog();

	private:
		std::mutex m_logMutex;
		std::deque<std::string> m_items;
		ImGuiTextFilter m_filter;
	};
}
