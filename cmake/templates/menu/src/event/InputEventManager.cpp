#include "event/InputEventManager.h"

#include "util/KeyboardUtil.h"

namespace INPUT_EVENT {

	void Manager::Register(Process* a_process)
	{
		if (a_process) {
			m_processes.insert(a_process);
		}
	}

	void Manager::Unregister(Process* a_process)
	{
		if (a_process) {
			m_processes.erase(a_process);
		}
	}

	bool Manager::Update(RE::InputEvent* const* a_event)
	{
		auto handle{ false };

		auto dispatch = [&](auto&& func) {
			for (auto* process : m_processes) {
				if (func(process)) {
					handle = true;
				}
			}
		};

		for (auto event = *a_event; event; event = event->next) {

			if (const auto charEvent = event->AsCharEvent()) {
				dispatch([&](Process* process) { return process->CharEvent(charEvent->keyCode); });
			}
			else if (const auto buttonEvent = event->AsButtonEvent()) {
				auto scanCode = buttonEvent->GetIDCode();
				std::uint32_t virtualKey = MapVirtualKeyEx(scanCode, MAPVK_VSC_TO_VK_EX, GetKeyboardLayout(0));
				UTIL::CorrectExtendedKeys(scanCode, &virtualKey);

				switch (event->GetDevice()) {
					case RE::INPUT_DEVICE::kGamepad: {
						dispatch([&](Process* process) { return process->GamepadButtonEvent(buttonEvent); });
						break;
					}
					case RE::INPUT_DEVICE::kKeyboard: {
						dispatch([&](Process* process) { return process->KeyboardEvent(buttonEvent, virtualKey); });
						break;
					}
					case RE::INPUT_DEVICE::kMouse: {
						dispatch([&](Process* process) { return process->MouseButtonEvent(buttonEvent); });
						break;
					}
				}
			}
			else if (const auto mouseMoveEvent = event->AsMouseMoveEvent()) {
				dispatch([&](Process* process) { return process->MouseMoveEvent(mouseMoveEvent); });
			}
			else if (const auto thumbstickEvent = event->AsThumbstickEvent()) {
				dispatch([&](Process* process) { return process->ThumbstickEvent(thumbstickEvent); });
			}
		}
		return handle;
	}
}
