#include "menu/IMenu.h"

namespace Menu {

	void RegisterMenu::Register(IMenu* a_menu)
	{
		m_menus.emplace_back(a_menu);
	}

	void RegisterMenu::Close()
	{
		for (IMenu* menu : m_menus) {
			menu->Close();
		}
	}

	void RegisterMenu::Open()
	{
		for (IMenu* menu : m_menus) {
			menu->Open();
		}
	}

	void RegisterMenu::Update()
	{
		for (IMenu* menu : m_menus) {
			menu->Update();
		}
	}

	void RegisterMenu::Hotkey()
	{
		for (IMenu* menu : m_menus) {
			menu->Hotkey();
		}
	}
}
