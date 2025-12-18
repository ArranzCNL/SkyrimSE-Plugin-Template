#include "hook/GraphicsHook.h"

#include "util/LogUtil.h"
#include "util/StringUtil.h"

namespace HOOK {

	struct WndProc {

		static LRESULT thunk(REX::W32::HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			if (uMsg == WM_KILLFOCUS) {

				auto graphics = Graphics::GetSingleton();
				if (graphics->IsMenuDisplayed(UI::DISPLAY_MODE::kMainMenu)) {
					graphics->DisplayMenu(UI::DISPLAY_MODE::kMainMenu, false);
				}
			}
			return func(hWnd, uMsg, wParam, lParam);
		}
		static inline REX::W32::WNDPROC func;
	};

	struct CreateGraphicsDevice {

		static void thunk()
		{
			func();

			auto graphics = Graphics::GetSingleton();
			auto renderer = RE::BSGraphics::Renderer::GetSingleton();
			const auto renderData = PROXY::BSGraphics::Renderer::Data(renderer);

			graphics->m_info.device = renderData->forwarder;
			graphics->m_info.deviceContext = renderData->context;
			graphics->m_info.swapChain = renderData->renderWindows[0].swapChain;

			REX::W32::DXGI_SWAP_CHAIN_DESC desc{};
			graphics->m_info.swapChain->GetDesc(&desc);

			if (FAILED(graphics->m_info.swapChain->GetDesc(std::addressof(desc)))) {
				LOG_ERROR("[IDXGISwapChain::GetDesc] Failure");
				return;
			}

			graphics->m_info.windowHandle = desc.outputWindow;
			graphics->m_info.windowHeight = desc.bufferDesc.height;
			graphics->m_info.windowWidth = desc.bufferDesc.width;
			graphics->m_info.windowPosX = renderData->renderWindows[0].windowX;
			graphics->m_info.windowPosY = renderData->renderWindows[0].windowY;

			if (HWND hwnd = reinterpret_cast<HWND>(graphics->m_info.windowHandle)) {

				if (std::int32_t txtLen = GetWindowTextLengthW(hwnd)) {
					std::vector<wchar_t> buffer(txtLen + 1);

					if (GetWindowTextW(hwnd, buffer.data(), static_cast<int>(buffer.size()))) {
						graphics->m_info.windowTitle = UTIL::STRING::WideToUTF8(buffer.data());
					}
				}
			}

			graphics->m_ui = std::make_unique<UI::Manager>();
			WndProc::func = reinterpret_cast<REX::W32::WNDPROC>(REX::W32::SetWindowLongPtrA(graphics->Info().windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc::thunk)));
			graphics->m_info.loaded = true;
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	struct DXGIPresent {

		static void thunk(std::uint32_t a_timer)
		{
			func(a_timer);

			static auto graphics = Graphics::GetSingleton();

			if (graphics->IsMenuDisplayed(UI::DISPLAY_MODE::kMainMenu) || graphics->IsMenuDisplayed(UI::DISPLAY_MODE::kConsole)) {
				graphics->m_ui->Process();
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	void Graphics::Install()
	{
		LOG_INFO("  Graphics");
		stl::Hook::call<CreateGraphicsDevice>(RELOCATION_ID(75595, 77226).address() + RELOCATION_OFFSET(0x9, 0x275));
		stl::Hook::call<DXGIPresent>(RELOCATION_ID(75461, 77246).address() + 0x9);
	}

	bool Graphics::IsMenuDisplayed(const UI::DISPLAY_MODE a_mode)
	{
		bool displayed = false;

		if (auto ui = m_ui.get()) {
			if (ui->IsDisplayed(a_mode)) {
				displayed = true;
			}
		}
		return displayed;
	}

	void Graphics::DisplayMenu(const UI::DISPLAY_MODE a_mode, bool a_enable)
	{
		if (auto ui = m_ui.get()) {
			ui->DisplayUI(a_mode, a_enable);
		}
	}
}
