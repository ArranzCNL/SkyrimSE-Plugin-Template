#include "hook/InputHook.h"

#include "console/ConsoleCommands.h"

namespace Hook {

	namespace Input {

		static constexpr auto CONSOLE_PREFIX = "test";

		struct ProcessConsole {

			static void thunk(RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name, RE::TESObjectREFR* a_objectRefr)
			{
				const std::string text = a_script->text;
				// Search for specific prefix.
				if (text.starts_with(CONSOLE_PREFIX)) {
					Console::Commands::Plugin(a_script);
					return;
				}

				func(a_script, a_compiler, a_name, a_objectRefr);
			}
			static inline REL::Relocation<decltype(thunk)> func;
			static inline constexpr std::size_t size{ 0x5 };
		};

		void Install()
		{
			// Called when ProcessConsole is updated
			stl::write_thunk_call<ProcessConsole>(RELOCATION_ID(52065, 52952).address() + RELOCATION_OFFSET(0xE2, 0x52));
		}
	}
}
