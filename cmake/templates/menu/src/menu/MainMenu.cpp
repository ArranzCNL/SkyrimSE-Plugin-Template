#include "menu/MainMenu.h"

#include "menu/IMenu.h"
#include "hook/GraphicsHook.h"
#include "plugin.h"

namespace Menu {

	Main::Main()
	{
		m_plugin = DLLMain::Plugin::GetSingleton();
		m_graphics = Hook::Graphics::GetSingleton();

		m_titlebarName = std::format("{} v{}", m_plugin->Info().name, m_plugin->Info().version.string("."));

		// Register Menus
		m_playerMenu = std::make_unique<Player>();
	}

	void Main::Close()
	{
		auto registeredMenus = RegisterMenu::GetSingleton();
		registeredMenus->Close();
	}

	void Main::Open()
	{

	}

	void Main::Update()
	{
		this->ShowMenuBar();
		this->ShowMenus();

		auto registeredMenus = RegisterMenu::GetSingleton();
		registeredMenus->Update();
	}

	void Main::KeyboardInput()
	{
		// Close menu, Esc.
		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			m_graphics->DisplayMenu(DISPLAY_MODE::kMainMenu, false);
		}
		auto registeredMenus = RegisterMenu::GetSingleton();
		registeredMenus->Hotkey();
	}

	void Main::ShowMenuBar()
	{
		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("File")) {

				ImGui::Text(ICON_FA_RIGHT_FROM_BRACKET);
				ImGui::SameLine();
				if (ImGui::MenuItem("Close", "Esc")) {
					m_graphics->DisplayMenu(DISPLAY_MODE::kMainMenu, false);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {

				ImGui::TextColored(ImVec4(0.314f, 0.6f, 0.894f, 1.0f), ICON_FA_MICROCHIP);
				ImGui::SameLine();
				if (ImGui::MenuItem("Plugin Info")) {
					m_viewPluginInfoWindow = !m_viewPluginInfoWindow;
				}
				ImGui::Separator();
				auto registeredMenus = RegisterMenu::GetSingleton();
				registeredMenus->Open();

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools")) {

				ImGui::Text(ICON_FA_TERMINAL);
				ImGui::SameLine();
				if (ImGui::MenuItem("Console")) {
					auto enable = !m_graphics->IsMenuDisplayed(Menu::DISPLAY_MODE::kConsole);
					m_graphics->DisplayMenu(DISPLAY_MODE::kConsole, enable);
				}

				ImGui::TextColored(ImVec4(0.961f, 0.341f, 0.384f, 1.0f), ICON_FA_BUG);
				ImGui::SameLine();
				if (ImGui::MenuItem("Stack Tool")) {
					m_viewStackToolWindow = !m_viewStackToolWindow;
				}

				ImGui::TextColored(ImVec4(0.314f, 0.6f, 0.894f, 1.0f), ICON_FA_SCREWDRIVER_WRENCH);
				ImGui::SameLine();
				if (ImGui::MenuItem("Metrics/Debugger")) {
					m_viewMetricsWindow = !m_viewMetricsWindow;
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Style Editor")) {
					m_viewStyleEditorWindow = !m_viewStyleEditorWindow;
				}

				if (ImGui::MenuItem("ImGui Demo")) {
					m_viewDemoWindow = !m_viewDemoWindow;
				}

				ImGui::EndMenu();
			}
			// Help
			if (ImGui::BeginMenu("Help")) {

				ImGui::TextColored(ImVec4(0.314f, 0.6f, 0.894f, 1.0f), ICON_FA_CIRCLE_INFO);
				ImGui::SameLine();
				if (ImGui::MenuItem("About Dear ImGui")) {
					m_viewAboutImGuiWindow = !m_viewAboutImGuiWindow;
				}

				ImGui::EndMenu();
			}
			// Setup titlebar name aligned to the right
			//    Calculate the width of m_titlebarName along with the fontsize so it does not get cutoff.
			static auto titleNameWidth = ImGui::CalcTextSize(m_titlebarName.c_str()).x + (ImGui::GetFontSize() + 2.0f);
			ImGui::SameLine(ImGui::GetWindowWidth() - titleNameWidth);
			ImGui::Text(m_titlebarName.c_str());

			ImGui::EndMainMenuBar();
		}
	}

	void Main::ShowMenus()
	{
		// View
		if (m_viewPluginInfoWindow) {

			ImGui::Begin("Plugin Info", &m_viewPluginInfoWindow, ImGuiWindowFlags_NoCollapse);

			if (ImGui::BeginTable("PluginTableInfo", 2, ImGuiTableFlags_SizingFixedFit)) {

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text(m_plugin->Info().name.c_str());
				ImGui::TableNextColumn();
				ImGui::PushItemWidth(250);
				ImGui::Text(m_plugin->Info().version.string(".").c_str());
				ImGui::PopItemWidth();

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Skyrim Script Extender");
				ImGui::TableNextColumn();
				ImGui::PushItemWidth(250);
				ImGui::Text(m_plugin->Info().skseVersion.string(".").c_str());
				ImGui::PopItemWidth();

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text(m_graphics->Info().windowTitle.c_str());
				ImGui::TableNextColumn();
				ImGui::PushItemWidth(250);
				ImGui::Text(m_plugin->Info().gameVersion.string(".").c_str());
				ImGui::PopItemWidth();

				ImGui::EndTable();
			}
			ImGui::End();
		}
		// Tools
		if (m_viewStackToolWindow) {
			ImGui::ShowStackToolWindow(&m_viewStackToolWindow);
		}

		if (m_viewMetricsWindow) {
			ImGui::ShowMetricsWindow(&m_viewMetricsWindow);
		}

		if (m_viewStyleEditorWindow) {
			ImGui::Begin("Style Editor", &m_viewStyleEditorWindow);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}

		if (m_viewDemoWindow) {
			ImGui::ShowDemoWindow(&m_viewDemoWindow);
		}
		// Help
		if (m_viewAboutImGuiWindow) {
			ImGui::ShowAboutWindow(&m_viewAboutImGuiWindow);
		}
	}
}
