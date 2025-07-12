#pragma once

#include "menu/ImguiHelper.h"

namespace Menu {

	class Main;

	class IMenu {

	public:
		IMenu() = default;
		virtual ~IMenu() = default;

		virtual void Close() = 0;
		virtual void Open() = 0;
		virtual void Update() = 0;
		virtual void Hotkey() = 0;

	protected:
		bool m_display = false;
	};

	class RegisterMenu final : public REX::Singleton<RegisterMenu> {

	public:
		void Register(IMenu* a_menu);

	private:
		void Close();
		void Open();
		void Update();
		void Hotkey();

	private:
		std::vector<IMenu*> m_menus;

		friend class Main;
	};
}
