#include "input/InputManager.h"

#include "util/KeyboardUtil.h"

namespace Input {

	void Manager::Register(Process* a_process)
	{
		m_processes.emplace_back(a_process);
	}

	bool Manager::Update(RE::InputEvent* const* a_event)
	{
		auto handle{ false };

		for (auto event = *a_event; event; event = event->next) {

			if (const auto charEvent = event->AsCharEvent()) {
				for (Process* process : m_processes) {

					if (process->CharEvent(charEvent->keyCode)) {
						handle = true;
					}
				}
			}
			else if (const auto buttonEvent = event->AsButtonEvent()) {
				auto scanCode = buttonEvent->GetIDCode();
				std::uint32_t virtualKey = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0));
				Util::CorrectExtendedKeys(scanCode, &virtualKey);

				switch (event->GetDevice()) {
					case RE::INPUT_DEVICE::kGamepad: {
						for (Process* process : m_processes) {

							if (process->GamepadButtonEvent(buttonEvent)) {
								handle = true;
							}
						}
						break;
					}
					case RE::INPUT_DEVICE::kKeyboard: {
						for (Process* process : m_processes) {

							if (process->KeyboardEvent(buttonEvent, virtualKey)) {
								handle = true;
							}
						}
						break;
					}
					case RE::INPUT_DEVICE::kMouse: {

						for (Process* process : m_processes) {

							if (process->MouseButtonEvent(buttonEvent)) {
								handle = true;
							}
						}
						break;
					}
				}
			}
			else if (const auto mouseMoveEvent = event->AsMouseMoveEvent()) {
				for (Process* process : m_processes) {

					if (process->MouseMoveEvent(mouseMoveEvent)) {
						handle = true;
					}
				}
			}
			else if (const auto thumbstickEvent = event->AsThumbstickEvent()) {
				for (Process* process : m_processes) {

					if (process->ThumbstickEvent(thumbstickEvent)) {
						handle = true;
					}
				}
			}
		}
		return handle;
	}
}
