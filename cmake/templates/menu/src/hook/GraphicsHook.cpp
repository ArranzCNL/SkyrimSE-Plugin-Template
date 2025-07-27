#include "hook/GraphicsHook.h"

#include "util/LogUtil.h"

namespace Hook {

	struct WndProc {

		static LRESULT thunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (uMsg == WM_KILLFOCUS) {

				auto graphics = Graphics::GetSingleton();
				if (graphics->IsMenuDisplayed(Menu::DISPLAY_MODE::kMainMenu)) {
					graphics->DisplayMenu(Menu::DISPLAY_MODE::kMainMenu, false);
				}
			}
			return func(hWnd, uMsg, wParam, lParam);
		}
		static inline WNDPROC func;
	};

	struct DXGIPresent {

		static void thunk(std::uint32_t a_timer)
		{
			func(a_timer);

			static auto graphics = Graphics::GetSingleton();

			std::call_once(graphics->m_graphicsFlag, []() {

				auto renderer = RE::BSGraphics::Renderer::GetSingleton();
				const auto renderData = PROXY::BSGraphics::Renderer::Data(renderer);
				REX::W32::GUID const& rexGUID = *reinterpret_cast<const REX::W32::GUID*>(&__uuidof(REX::W32::ID3D11Device));

				graphics->m_info.swapChain = renderData->renderWindows[0].swapChain;

				if (SUCCEEDED(graphics->m_info.swapChain->GetDevice(rexGUID, &graphics->m_info.device))) {
					graphics->m_info.device->GetImmediateContext(&graphics->m_info.deviceContext);
					graphics->m_info.windowHandle = reinterpret_cast<HWND>(renderData->renderWindows[0].hWnd);
					graphics->m_info.windowWidth = renderData->renderWindows[0].windowWidth;
					graphics->m_info.windowHeight = renderData->renderWindows[0].windowHeight;
					graphics->m_info.windowPosX = renderData->renderWindows[0].windowX;
					graphics->m_info.windowPosY = renderData->renderWindows[0].windowY;

					char windowTitle[MAX_PATH]{};
					GetWindowTextA(graphics->m_info.windowHandle, windowTitle, sizeof(windowTitle));
					graphics->m_info.windowTitle = windowTitle;
					graphics->m_ui = std::make_unique<Menu::UI>();

					WndProc::func = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(graphics->Info().windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc::thunk)));
					graphics->m_info.loaded = true;
				}
			});

			if (graphics->IsMenuDisplayed(Menu::DISPLAY_MODE::kMainMenu) || graphics->IsMenuDisplayed(Menu::DISPLAY_MODE::kConsole)) {
				graphics->m_ui->BeginFrame();
				graphics->m_ui->OnUpdate();
				graphics->m_ui->EndFrame();
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;
		static inline constexpr std::size_t size{ 0x5 };
	};

	void Graphics::Install()
	{
		LOG_INFO("  Graphics");
		stl::write_thunk_call<DXGIPresent>(RELOCATION_ID(75461, 77246).address() + 0x9);
	}

	bool Graphics::IsMenuDisplayed(const Menu::DISPLAY_MODE a_mode)
	{
		bool displayed = false;

		if (auto ui = m_ui.get()) {
			if (ui->IsDisplayed(a_mode)) {
				displayed = true;
			}
		}
		return displayed;
	}

	void Graphics::DisplayMenu(const Menu::DISPLAY_MODE a_mode, bool a_enable)
	{
		if (auto ui = m_ui.get()) {
			ui->DisplayUI(a_mode, a_enable);
		}
	}
}
