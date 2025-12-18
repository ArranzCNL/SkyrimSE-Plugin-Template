#pragma once

#include "ui/menu/Menus.h"

namespace DLLMAIN {
	class Plugin;
}

namespace HOOK {
	class Graphics;
}

namespace MENU {
	
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
		DLLMAIN::Plugin* m_plugin{ nullptr };
		HOOK::Graphics* m_graphics{ nullptr };

		std::string m_titlebarName{};
		bool m_viewPluginInfoWindow{ false };
		bool m_viewStackToolWindow{ false };
		bool m_viewMetricsWindow{ false };
		bool m_viewStyleEditorWindow{ false };
		bool m_viewDemoWindow{ false };
		bool m_viewAboutImGuiWindow{ false };

	private:
		std::unique_ptr<PlayerMenu> m_playerMenu{ nullptr };
	};
}
