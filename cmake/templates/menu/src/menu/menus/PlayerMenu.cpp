#include "menu/menus/PlayerMenu.h"

namespace Menu {

	Player::Player()
	{
		RegisterMenu::GetSingleton()->Register(this);
	}

	void Player::Open()
	{
		ImGui::TextColored(ImVec4(0.573f, 0.894f, 0.573f, 1.0f), ICON_FA_USER);
		ImGui::SameLine();
		if (ImGui::MenuItem("Player", "Ctrl+P")) {
			m_display = !m_display;
		}
	}

	void Player::Update()
	{
		if (!m_display) {
			return;
		}

		if (ImGui::Begin("Player", &m_display, ImGuiWindowFlags_NoCollapse)) {

			ImGui::Text("Simply Player Menu");

			ImGui::End();
		}
	}

	void Player::Close()
	{
		// Remove if you want the menu to reopen when main menu does.
		m_display = false;
	}

	void Player::Hotkey()
	{
		// Player Menu, CTRL + P
		if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_P, ImGuiInputFlags_None, ImGuiKeyOwner_NoOwner)) {
			m_display = true;
		}
	}
}
