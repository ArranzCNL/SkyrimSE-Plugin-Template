#pragma once

namespace UTIL {

	static inline void CorrectExtendedKeys(const std::uint32_t a_scanCode, std::uint32_t* a_virtualKey)
	{
		switch (a_scanCode) {

			case REX::W32::DIK_LWIN: {
				*a_virtualKey = VK_LWIN;
				break;
			}
			case REX::W32::DIK_RMENU: {
				*a_virtualKey = VK_RMENU;
				break;
			}
			case REX::W32::DIK_RWIN: {
				*a_virtualKey = VK_RWIN;
				break;
			}
			case REX::W32::DIK_APPS: {
				*a_virtualKey = VK_APPS;
				break;
			}
			case REX::W32::DIK_RCONTROL: {
				*a_virtualKey = VK_RCONTROL;
				break;
			}
			case REX::W32::DIK_SYSRQ: {
				// Print Screen
				*a_virtualKey = VK_SNAPSHOT;
				break;
			}
			// Not an extended key
			case REX::W32::DIK_PAUSE: {
				*a_virtualKey = VK_PAUSE;
				break;
			}
			case REX::W32::DIK_INSERT: {
				*a_virtualKey = VK_INSERT;
				break;
			}
			case REX::W32::DIK_HOME: {
				*a_virtualKey = VK_HOME;
				break;
			}
			case REX::W32::DIK_PRIOR: {
				// Page Up
				*a_virtualKey = VK_PRIOR;
				break;
			}
			case REX::W32::DIK_DELETE: {
				*a_virtualKey = VK_DELETE;
				break;
			}
			case REX::W32::DIK_END: {
				*a_virtualKey = VK_END;
				break;
			}
			case REX::W32::DIK_NEXT: {
				// Page Down
				*a_virtualKey = VK_NEXT;
				break;
			}
			case REX::W32::DIK_UP: {
				*a_virtualKey = VK_UP;
				break;
			}
			case REX::W32::DIK_LEFT: {
				*a_virtualKey = VK_LEFT;
				break;
			}
			case REX::W32::DIK_DOWN: {
				*a_virtualKey = VK_DOWN;
				break;
			}
			case REX::W32::DIK_RIGHT: {
				*a_virtualKey = VK_RIGHT;
				break;
			}
			case REX::W32::DIK_DIVIDE: {
				*a_virtualKey = VK_DIVIDE;
				break;
			}
			case REX::W32::DIK_NUMPAD7: {
				*a_virtualKey = VK_NUMPAD7;
				break;
			}
			case REX::W32::DIK_NUMPAD8: {
				*a_virtualKey = VK_NUMPAD8;
				break;
			}
			case REX::W32::DIK_NUMPAD9: {
				*a_virtualKey = VK_NUMPAD9;
				break;
			}
			case REX::W32::DIK_NUMPAD4: {
				*a_virtualKey = VK_NUMPAD4;
				break;
			}
			case REX::W32::DIK_NUMPAD5: {
				*a_virtualKey = VK_NUMPAD5;
				break;
			}
			case REX::W32::DIK_NUMPAD6: {
				*a_virtualKey = VK_NUMPAD6;
				break;
			}
			case REX::W32::DIK_NUMPAD1: {
				*a_virtualKey = VK_NUMPAD1;
				break;
			}
			case REX::W32::DIK_NUMPAD2: {
				*a_virtualKey = VK_NUMPAD2;
				break;
			}
			case REX::W32::DIK_NUMPAD3: {
				*a_virtualKey = VK_NUMPAD3;
				break;
			}
			case REX::W32::DIK_NUMPAD0: {
				*a_virtualKey = VK_NUMPAD0;
				break;
			}
			case REX::W32::DIK_DECIMAL: {
				*a_virtualKey = VK_DECIMAL;
				break;
			}
			case REX::W32::DIK_NUMPADENTER: {
				*a_virtualKey = VK_RETURN + 256;  // Numpad Enter (Special handling)
				break;
			}
			// Unsure of this one should be fine.
			case REX::W32::DIK_NUMPADEQUALS: {
				*a_virtualKey = VK_OEM_NEC_EQUAL;
				break;
			}
		}
	}
}
