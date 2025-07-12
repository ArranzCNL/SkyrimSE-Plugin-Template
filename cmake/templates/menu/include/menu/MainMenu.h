#pragma once

#include "menu/Menus.h"

namespace DLLMain {
	class Plugin;
}

namespace Hook {
	class Graphics;
}

namespace Menu {
	
	class Main final {

	public:
		Main();
		virtual ~Main() = default;

		void Close();
		void Open();
		void Update();
		void KeyboardInput();

	private:
		void ShowMenuBar();
		void ShowMenus();

	private:
		DLLMain::Plugin* m_plugin{ nullptr };
		Hook::Graphics* m_graphics{ nullptr };

		std::string m_titlebarName{};
		bool m_viewPluginInfoWindow{ false };
		bool m_viewStackToolWindow{ false };
		bool m_viewMetricsWindow{ false };
		bool m_viewStyleEditorWindow{ false };
		bool m_viewDemoWindow{ false };
		bool m_viewAboutImGuiWindow{ false };

	private:
		std::unique_ptr<Player> m_playerMenu{ nullptr };
	};
}
