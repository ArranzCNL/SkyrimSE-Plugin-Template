#include "game/CommandsGame.h"

#include "plugin.h"
#include "hook/FunctionHook.h"
#include "game/HelperGame.h"

namespace Game {

	namespace Commands {

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

			auto pluginInfo = &DLLMain::Plugin::GetSingleton()->Info();
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
			// Display current running animations for third person
			if (command.compare("anim") == 0) {
				if (CheckConsole()) {
					Game::Helper::GetCurrentAnimations();
				}
			}
			// Fade out the game for 3s
			else if (command.compare("fade") == 0) {
				if (CheckConsole(true)) {
					Hook::Function::FadeOutGame(false, true, 1.0f, true, 3.0f);
				}
			}
			// Display current delta time
			else if (command.compare("delta") == 0) {
				consoleLog->Print("Current delta time is >> %f", Helper::Get_DeltaWorldTime());
			}
			// Display current stack trace
			else if (command.compare("trace") == 0) {
				consoleLog->Print("Dumping current stack trace to debug console");
				Helper::StackTrace();
			}
			// Unknown command
			else {
				consoleLog->Print("%s: unknown command '%s'", pluginInfo->name.c_str(), command.c_str());
			}
		}
	}
}
