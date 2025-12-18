#include "ui/UIManager.h"

#include "ui/ImguiHelper.h"
#include "plugin.h"

namespace UI {

	Manager::Manager()
	{
		// Register for input updates.
		INPUT_EVENT::Manager::GetSingleton()->Register(this);
		// Store menu layout(s), comment this line out if you do not want this.
		m_editorFile = DLLMAIN::Plugin::GetSingleton()->Info().directory + "editorconfig.ini";
		// Setup context
		ImGui::SetupContext(m_editorFile);
		// Setup platform/renderer backends
		ImGui::SetupBackend();
		// Setup builtin menus
		m_consoleMenu = std::make_unique<MENU::Console>();
		m_mainMenu = std::make_unique<MENU::Main>();
	}

	Manager::~Manager()
	{
		ImGui::ShutDown();
	}

	void Manager::Process()
	{
		this->BeginFrame();
		this->OnUpdate();
		this->EndFrame();
	}

	bool Manager::CharEvent(std::uint32_t a_char)
	{
		if (!m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
			return false;
		}

		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(a_char);
		return true;
	}

	bool Manager::GamepadButtonEvent(RE::ButtonEvent* const)
	{
		if (!m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
			return false;
		}

		return true;
	}

	bool Manager::KeyboardEvent(RE::ButtonEvent* const a_buttonEvent, std::uint32_t a_virtualKey)
	{
		if (!m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
			return false;
		}

		ImGuiIO& io = ImGui::GetIO();
		auto buttonPressed = a_buttonEvent->IsPressed();
		// Send modifier key event.
		if (a_virtualKey == VK_LSHIFT || a_virtualKey == VK_RSHIFT) {
			io.AddKeyEvent(ImGuiMod_Shift, buttonPressed);
		}
		else if (a_virtualKey == VK_LCONTROL || a_virtualKey == VK_RCONTROL) {
			io.AddKeyEvent(ImGuiMod_Ctrl, buttonPressed);
		}
		else if (a_virtualKey == VK_LMENU || a_virtualKey == VK_RMENU) {
			io.AddKeyEvent(ImGuiMod_Alt, buttonPressed);
		}
		// Send key event.
		io.AddKeyEvent(ImGui::VirtualKeyToImGuiKey(a_virtualKey), buttonPressed);
		m_mainMenu->KeyboardInput();
		return true;
	}

	bool Manager::MouseButtonEvent(RE::ButtonEvent* const a_buttonEvent)
	{
		if (!m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
			return false;
		}

		ImGuiIO& io = ImGui::GetIO();
		// Mouse Wheel: 8 is Foward, 9 is Back
		auto scanCode = a_buttonEvent->GetIDCode();
		if (scanCode == 8 || scanCode == 9) {
			io.AddMouseWheelEvent(0.0f, a_buttonEvent->Value() * (scanCode == 8 ? 1.0f : -1.0f));
		}
		else {
			// Only handle 5 mouse buttons
			if (scanCode > 4) {
				scanCode = 4;
			}
			io.AddMouseButtonEvent(scanCode, a_buttonEvent->IsPressed());
		}
		return true;
	}

	bool Manager::MouseMoveEvent(RE::MouseMoveEvent* const)
	{
		if (!m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
			return false;
		}

		return true;
	}

	bool Manager::ThumbstickEvent(RE::ThumbstickEvent* const)
	{
		if (!m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
			return false;
		}

		return true;
	}

	void Manager::DisplayUI(const DISPLAY_MODE a_mode, bool a_enable)
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

	void Manager::ConsoleLog(std::string_view a_msg)
	{
		m_consoleMenu->AddLog(a_msg.data());
	}

	void Manager::BeginFrame()
	{
		ImGui::StartFrame();
	}

	void Manager::OnUpdate()
	{
		if (m_displayUI.all(DISPLAY_MODE::kMainMenu)) {
			m_mainMenu->Update();
		}

		if (m_displayUI.all(DISPLAY_MODE::kConsole)) {
			m_consoleMenu->Update();
		}
	}

	void Manager::EndFrame()
	{
		ImGui::FinalFrame();
	}

	void Manager::Open()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = true;

		m_mainMenu->Open();
	}

	void Manager::Close()
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
