#pragma once

#include "input/InputManager.h"
#include "menu/ConsoleMenu.h"
#include "menu/MainMenu.h"

namespace Menu {

	enum class DISPLAY_MODE {
		kNone = 0,
		kMainMenu = 1 << 0,
		kConsole = 1 << 1,

		kAll = static_cast<std::underlying_type_t<DISPLAY_MODE>>(-1)
	};

	class Manager final : public Input::Process {

	public:
		Manager();
		virtual ~Manager();

		void BeginFrame();
		void OnUpdate();
		void EndFrame();

		void DisplayUI(const DISPLAY_MODE a_mode, bool a_enable);
		constexpr bool IsDisplayed(DISPLAY_MODE a_mode) const noexcept { return m_displayUI.all(a_mode); }
		void ConsoleLog(std::string_view a_msg);
		// Input::Process
		virtual bool CharEvent(std::uint32_t a_char) override;
		virtual bool GamepadButtonEvent(RE::ButtonEvent* const a_buttonEvent) override;
		virtual bool KeyboardEvent(RE::ButtonEvent* const a_buttonEvent, std::uint32_t a_virtualKey) override;
		virtual bool MouseButtonEvent(RE::ButtonEvent* const a_buttonEvent) override;
		virtual bool MouseMoveEvent(RE::MouseMoveEvent* const a_mouseMoveEvent) override;
		virtual bool ThumbstickEvent(RE::ThumbstickEvent* const a_thumbstickEvent) override;

	private:
		void Open();
		void Close();

	private:
		REX::EnumSet<DISPLAY_MODE, std::uint32_t> m_displayUI;
		std::string m_editorFile{};

		std::unique_ptr<Main> m_mainMenu{ nullptr };
		std::unique_ptr<Console> m_consoleMenu{ nullptr };
	};
}
