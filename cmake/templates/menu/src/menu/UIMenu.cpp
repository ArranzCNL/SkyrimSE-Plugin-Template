#include "menu/UIMenu.h"

#include "menu/ImguiHelper.h"
#include "plugin.h"

namespace Menu {

	UI::UI()
	{
		// Register for input updates.
		Game::RegisterInput::GetSingleton()->Register(this);
		// Store menu layout(s), comment this line out if you do not want this.
		m_editorFile = DLLMain::Plugin::GetSingleton()->Info().directory + "editorconfig.ini";
		// Setup context
		ImGui::SetupContext(m_editorFile);
		// Setup platform/renderer backends
		ImGui::SetupBackend();
		// Setup builtin menus
		m_consoleMenu = std::make_unique<Console>();
		m_mainMenu = std::make_unique<Main>();
	}

	UI::~UI()
	{
		ImGui::ShutDown();
	}

	void UI::BeginFrame()
	{
		ImGui::StartFrame();
	}

	void UI::OnUpdate()
	{
		if (m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
			m_mainMenu->Update();
		}

		if (m_displayUI.all(DISPLAY_MODE::kConsole)) {
			m_consoleMenu->Update();
		}
	}

	void UI::EndFrame()
	{
		ImGui::FinalFrame();
	}

	bool UI::ProcessInput(RE::InputEvent* const* a_event)
	{
		if (!m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
			return false;
		}

		ImGuiIO& io = ImGui::GetIO();

		for (auto event = *a_event; event; event = event->next) {

			if (const auto charEvent = event->AsCharEvent()) {
				io.AddInputCharacter(charEvent->keyCode);
			}
			else if (const auto buttonEvent = event->AsButtonEvent()) {
				auto scanCode = buttonEvent->GetIDCode();
				auto virtualKey = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0));
				Util::CorrectExtendedKeys(scanCode, &virtualKey);
				auto buttonPressed = buttonEvent->IsPressed();

				switch (event->GetDevice()) {

					case RE::INPUT_DEVICE::kKeyboard: {
						// Send modifier key event.
						if (virtualKey == VK_LSHIFT || virtualKey == VK_RSHIFT) {
							io.AddKeyEvent(ImGuiMod_Shift, buttonPressed);
						}
						else if (virtualKey == VK_LCONTROL || virtualKey == VK_RCONTROL) {
							io.AddKeyEvent(ImGuiMod_Ctrl, buttonPressed);
						}
						else if (virtualKey == VK_LMENU || virtualKey == VK_RMENU) {
							io.AddKeyEvent(ImGuiMod_Alt, buttonPressed);
						}
						// Send key event.
						io.AddKeyEvent(ImGui::VirtualKeyToImGuiKey(virtualKey), buttonPressed);
						m_mainMenu->KeyboardInput();

						break;
					}
					case RE::INPUT_DEVICE::kMouse: {
						// Mouse Wheel: 8 is Foward, 9 is Back
						if (scanCode == 8 || scanCode == 9) {
							io.AddMouseWheelEvent(0.0f, buttonEvent->Value() * (scanCode == 8 ? 1.0f : -1.0f));
						}
						else {
							// Only handle 5 mouse buttons
							if (scanCode > 4) {
								scanCode = 4;
							}
							io.AddMouseButtonEvent(scanCode, buttonPressed);
						}
						break;
					}
				}
			}
		}
		return true;
	}

	void UI::DisplayUI(const DISPLAY_MODE a_mode, bool a_enable)
	{
		if (a_enable) {
			m_displayUI.set(a_mode);
			if (m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
				this->Open();
			}
		}
		else {
			m_displayUI.reset(a_mode);
			if (!m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
				this->Close();
			}
		}
	}

	void UI::ConsoleLog(std::string_view a_msg)
	{
		m_consoleMenu->AddLog(a_msg.data());
	}

	void UI::Open()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = true;

		m_mainMenu->Open();
	}

	void UI::Close()
	{
		m_mainMenu->Close();

		// Save menu layout
		if (!m_editorFile.empty()) {
			ImGui::SaveIniSettingsToDisk(m_editorFile.c_str());
		}
		// Clear ImGui input
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = false;
		io.ClearEventsQueue();
		io.ClearInputKeys();
	}
}
