#pragma once

#include "hook/MainHook.h"
#include "menu/UIMenu.h"

#include <wrl/client.h>

namespace Hook {

	struct GraphicsInfo {
		HWND windowHandle{ nullptr };
		std::string windowTitle{};
		std::int32_t windowPosX{ 0 };
		std::int32_t windowPosY{ 0 };
		std::int32_t windowWidth{ 0 };
		std::int32_t windowHeight{ 0 };
		Microsoft::WRL::ComPtr<REX::W32::ID3D11Device> device{ nullptr };
		Microsoft::WRL::ComPtr<REX::W32::ID3D11DeviceContext> deviceContext{ nullptr };
		Microsoft::WRL::ComPtr<REX::W32::IDXGISwapChain> swapChain{ nullptr };
		bool loaded{ false };
	};

	class Graphics final : public REX::Singleton<Graphics> {

	public:
		const GraphicsInfo& Info() { return m_info; };
		bool IsMenuDisplayed(const Menu::DISPLAY_MODE a_mode);
		void DisplayMenu(const Menu::DISPLAY_MODE a_mode, bool a_enable);

		template <typename... Args>
		void ConsoleLog(std::format_string<Args...> a_msg, Args&&... a_args)
		{
			if (auto ui = m_ui.get()) {
				std::string message = std::format(a_msg, std::forward<Args>(a_args)...);

				// Add timestamp: [HH:MM:SS.mmm]
				using namespace std::chrono;
				auto now = system_clock::now();
				auto time_t_now = system_clock::to_time_t(now);
				auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

				std::tm localTime{};
				localtime_s(&localTime, &time_t_now);

				std::string timestamp = std::format("[{:02}:{:02}:{:02}.{:03}] ", localTime.tm_hour, localTime.tm_min, localTime.tm_sec, ms.count());
				ui->ConsoleLog(timestamp + message);
			}
		}

	private:
		static void Install();

	private:
		GraphicsInfo m_info{};
		std::once_flag m_graphicsFlag;

		std::unique_ptr<Menu::UI> m_ui{ nullptr };

	private:
		friend struct DXGIPresent;
		friend void Hook::Main::Install();
	};
}
