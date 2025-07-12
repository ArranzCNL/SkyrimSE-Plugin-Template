#include "menu/ConsoleMenu.h"

#include "menu/ImguiHelper.h"
#include "hook/GraphicsHook.h"

namespace Menu {

	static constexpr size_t MAX_ITEMS = 4096;
	static constexpr size_t PRUNE_COUNT = 512;

	Console::Console()
	{
		ClearLog();
	}

	void Console::Update()
	{
		auto graphics = Hook::Graphics::GetSingleton();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

		if (!graphics->IsMenuDisplayed(DISPLAY_MODE::kMainMenu)) {
			flags |= ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
		}

		if (!ImGui::Begin("Console", nullptr, flags)) {
			ImGui::End();
			return;
		}

		std::lock_guard lock(m_logMutex);

		if (graphics->IsMenuDisplayed(DISPLAY_MODE::kMainMenu)) {
			ImGui::TextWrapped("Filter:");
			m_filter.Draw("##Filter", 180);
			ImGui::Separator();
		}

		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar)) {
			// Right click display Clear
			if (ImGui::BeginPopupContextWindow()) {
				if (ImGui::Selectable("Clear")) {
					ClearLog();
				}
				ImGui::EndPopup();
			}
			// Tighten spacing
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			for (const std::string& item : m_items) {
				if (!m_filter.PassFilter(item.c_str())) {
					continue;
				}

				ImVec4 color;
				bool has_color = false;
				if (item.find("[ERROR]") != std::string::npos) {
					color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
					has_color = true;
				}
				else if (item.find("[WARNING]") != std::string::npos) {
					color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
					has_color = true;
				}
				if (has_color) {
					ImGui::PushStyleColor(ImGuiCol_Text, color);
				}
				ImGui::TextUnformatted(item.c_str());
				if (has_color) {
					ImGui::PopStyleColor();
				}
			}

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
				ImGui::SetScrollHereY(1.0f);
			}
			ImGui::PopStyleVar();
		}
		ImGui::EndChild();
		ImGui::End();
	}

	void Console::ClearLog()
	{
		m_items.clear();
	}

	void Console::AddLog(std::string_view a_msg)
	{
		auto graphics = Hook::Graphics::GetSingleton();
		// Only update log when main menu is not displayed.
		if (!graphics->IsMenuDisplayed(DISPLAY_MODE::kMainMenu)) {
			std::lock_guard lock(m_logMutex);
			m_items.emplace_back(a_msg);

			if (m_items.size() > MAX_ITEMS + PRUNE_COUNT) {
				for (size_t i = 0; i < PRUNE_COUNT; ++i) {
					m_items.pop_front();
				}
			}
		}
	}
}
