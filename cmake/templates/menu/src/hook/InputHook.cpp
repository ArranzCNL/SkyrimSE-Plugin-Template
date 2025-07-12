#include "hook/InputHook.h"

#include "game/CommandsGame.h"
#include "game/InputGame.h"
#include "util/LogUtil.h"

namespace Hook {

	namespace Input {

		static constexpr auto CONSOLE_PREFIX = "test";

		struct ProcessConsole {

			static void thunk(RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name, RE::TESObjectREFR* a_objectRefr)
			{
				const std::string text = a_script->text;
				// Search for specific prefix.
				if (text.starts_with(CONSOLE_PREFIX)) {
					Game::Commands::Plugin(a_script);
					return;
				}

				func(a_script, a_compiler, a_name, a_objectRefr);
			}
			static inline REL::Relocation<decltype(thunk)> func;
			static inline constexpr std::size_t size{ 0x5 };
		};

		struct ProcessInput {

			static void thunk(RE::BSTEventSource<RE::InputEvent*>* a_dispatcher, RE::InputEvent* const* a_event)
			{
				constexpr RE::InputEvent* const dummy[]{ nullptr };
				auto handleInput{ false };

				// Error check.
				if (!a_event || !(*a_event)) {
					func(a_dispatcher, a_event);
					return;
				}
				// Only care about input when the console is not open.
				auto consoleOpen = RE::UI::GetSingleton()->IsMenuOpen(RE::Console::MENU_NAME);
				if (!consoleOpen) {
					handleInput = Game::RegisterInput::GetSingleton()->Update(a_event);
				}

				func(a_dispatcher, handleInput ? dummy : a_event);
			}
			static inline REL::Relocation<decltype(thunk)> func;
			static inline constexpr std::size_t size{ 0x5 };
		};

		void Install()
		{
			LOG_INFO("  Input");
			// Called when ProcessConsole is updated
			stl::write_thunk_call<ProcessConsole>(RELOCATION_ID(52065, 52952).address() + RELOCATION_OFFSET(0xE2, 0x52));
			// Called when ProcessInput is updated
			stl::write_thunk_call<ProcessInput>(RELOCATION_ID(67315, 68617).address() + 0x7B);
		}
	}
}
