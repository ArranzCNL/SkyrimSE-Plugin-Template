#pragma once

namespace Hook::Input {
	struct ProcessInput;
}

namespace Input {

	class Process {

	public:
		Process() = default;
		virtual ~Process() = default;

		virtual bool CharEvent(std::uint32_t /* a_char */) { return false; }
		virtual bool GamepadButtonEvent(RE::ButtonEvent* const /* a_buttonEvent */) { return false; }
		virtual bool KeyboardEvent(RE::ButtonEvent* const /* a_buttonEvent */, std::uint32_t /* a_virtualKey */) { return false; }
		virtual bool MouseButtonEvent(RE::ButtonEvent* const /* a_buttonEvent */) { return false; }
		virtual bool MouseMoveEvent(RE::MouseMoveEvent* const /* a_mouseMoveEvent */) { return false; }
		virtual bool ThumbstickEvent(RE::ThumbstickEvent* const /* a_thumbstickEvent */) { return false; }
	};

	class Manager final : public REX::Singleton<Manager> {

	public:
		void Register(Process* a_process);

	private:
		bool Update(RE::InputEvent* const* a_event);

	private:
		std::vector<Process*> m_processes;

		friend struct Hook::Input::ProcessInput;
	};
}
