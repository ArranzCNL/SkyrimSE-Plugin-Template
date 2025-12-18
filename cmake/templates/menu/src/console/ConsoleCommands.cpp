#include "console/ConsoleCommands.h"

#include "hook/GraphicsHook.h"
#include "plugin.h"

namespace CONSOLE::COMMANDS {

	static bool CheckConsole(bool a_close = false)
	{
		if (a_close) {
			const auto msgQueue = RE::UIMessageQueue::GetSingleton();
			msgQueue->AddMessage(RE::Console::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		}

		auto ui = RE::UI::GetSingleton();
		bool mainMenu = ui->IsMenuOpen(RE::MainMenu::MENU_NAME);
		bool loadingMenu = ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME);

		return (!mainMenu && !loadingMenu) ? true : false;
	}

	void Plugin(RE::Script* a_script)
	{
		// Read full console command entered.
		std::stringstream args(a_script->GetCommand());
		std::vector<std::string> consoleArgs{};
		std::string arg{};
		// Filter console command arguments.
		while (args >> arg) {
			// Restrict to a maximum of 5 arguments.
			if (consoleArgs.size() == 5) {
				break;
			}
			consoleArgs.emplace_back(arg);
		}
		std::size_t numArgs = consoleArgs.size() - 1;

		auto consoleLog = RE::ConsoleLog::GetSingleton();
		if (!consoleLog) {
			return;
		}

		auto pluginInfo = &DLLMAIN::Plugin::GetSingleton()->Info();
		// Return plugin version if no arguments are passed
		if (numArgs < 1) {
			consoleLog->Print("%s: version >> %s", pluginInfo->name.c_str(), pluginInfo->version.string(".").c_str());
			return;
		}

		auto const& command = consoleArgs[1];
		// Version
		if (command.compare("version") == 0) {
			consoleLog->Print("%s: version >> %s", pluginInfo->name.c_str(), pluginInfo->version.string(".").c_str());
			return;
		}
		// Open console
		if (command.compare("console") == 0) {
			if (auto graphics = HOOK::Graphics::GetSingleton()) {
				CheckConsole();
				auto enable = !graphics->IsMenuDisplayed(UI::DISPLAY_MODE::kConsole);
				graphics->DisplayMenu(UI::DISPLAY_MODE::kConsole, enable);
				graphics->ConsoleLog("{}: Console displayed? {}", pluginInfo->name.c_str(), enable);
			}
		}
		// Open menu
		else if (command.compare("menu") == 0) {
			if (auto graphics = HOOK::Graphics::GetSingleton()) {
				CheckConsole(true);
				graphics->DisplayMenu(UI::DISPLAY_MODE::kMainMenu, true);
			}
		}
		// Unknown command
		else {
			consoleLog->Print("%s: unknown command '%s'", pluginInfo->name.c_str(), command.c_str());
		}
	}
}
