#include "menu/ImguiHelper.h"

// imgui backends
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "menu/fonts/fa-solid-900.h"
#include "hook/GraphicsHook.h"

namespace ImGui {

	static void Theme()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = ImGui::GetStyle().Colors;

		//style->WindowTitleAlign = ImVec2(0.5, 0.5);
		style->FramePadding = ImVec2(4, 4);

		// Rounded slider grabber
		style->GrabRounding = 12.0f;

		// Window
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.118f, 0.118f, 0.118f, 0.784f };
		colors[ImGuiCol_ResizeGrip] = ImVec4{ 0.2f, 0.2f, 0.2f, 0.5f };
		colors[ImGuiCol_ResizeGripHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 0.75f };
		colors[ImGuiCol_ResizeGripActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Header
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Frame Background
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Button
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Tab
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
	}

	void SetupContext(std::string& a_editorFile)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NavEnableSetMousePos | ImGuiConfigFlags_NavEnableKeyboard;
		io.IniFilename = a_editorFile.empty() ? nullptr : a_editorFile.c_str();
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// Add default font
		io.Fonts->AddFontDefault();
		// Add fontawesome6 font
		ImFontConfig config;
		config.MergeMode = true;
		config.PixelSnapH = true;
		config.GlyphMinAdvanceX = 14.0f;
		static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		io.Fonts->AddFontFromMemoryTTF(fontAwesome6, sizeof(fontAwesome6), 14.0f, &config, icon_ranges);
		// Setup custom theme
		Theme();
	}

	void SetupBackend()
	{
		auto graphicsInfo = &Hook::Graphics::GetSingleton()->Info();
		ImGui_ImplWin32_Init(graphicsInfo->windowHandle);
		ImGui_ImplDX11_Init(reinterpret_cast<ID3D11Device*>(graphicsInfo->device.Get()), reinterpret_cast<ID3D11DeviceContext*>(graphicsInfo->deviceContext.Get()));
	}

	void StartFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		{
			// trick imgui into rendering at game's real resolution (ie. if upscaled with Display Tweaks)
			static const auto screenSize = RE::BSGraphics::Renderer::GetScreenSize();

			auto& io = ImGui::GetIO();
			io.DisplaySize.x = static_cast<float>(screenSize.width);
			io.DisplaySize.y = static_cast<float>(screenSize.height);
		}
		ImGui::NewFrame();
	}

	void FinalFrame()
	{
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void ShutDown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	const ImGuiKey VirtualKeyToImGuiKey(const std::uint32_t a_key)
	{
		switch (a_key) {

			case VK_TAB: {
				return ImGuiKey_Tab;
			}
			case VK_LEFT: {  // Left Arrow
				return ImGuiKey_LeftArrow;
			}
			case VK_RIGHT: {  // Right Arrow
				return ImGuiKey_RightArrow;
			}
			case VK_UP: {  // Up Arrow
				return ImGuiKey_UpArrow;
			}
			case VK_DOWN: {  // Down Arrow
				return ImGuiKey_DownArrow;
			}
			case VK_PRIOR: {  // Page Up
				return ImGuiKey_PageUp;
			}
			case VK_NEXT: {  // Page Down
				return ImGuiKey_PageDown;
			}
			case VK_HOME: {
				return ImGuiKey_Home;
			}
			case VK_END: {
				return ImGuiKey_End;
			}
			case VK_INSERT: {
				return ImGuiKey_Insert;
			}
			case VK_DELETE: {
				return ImGuiKey_Delete;
			}
			case VK_BACK: {
				return ImGuiKey_Backspace;
			}
			case VK_SPACE: {
				return ImGuiKey_Space;
			}
			case VK_RETURN: {
				return ImGuiKey_Enter;
			}
			case VK_ESCAPE: {
				return ImGuiKey_Escape;
			}
			case VK_LCONTROL: {
				return ImGuiKey_LeftCtrl;
			}
			case VK_LSHIFT: {
				return ImGuiKey_LeftShift;
			}
			case VK_LMENU: {  // Left Alt
				return ImGuiKey_LeftAlt;
			}
			case VK_LWIN: {
				return ImGuiKey_LeftSuper;
			}
			case VK_RCONTROL: {
				return ImGuiKey_RightCtrl;
			}
			case VK_RSHIFT: {
				return ImGuiKey_RightShift;
			}
			case VK_RMENU: {  // Right Alt
				return ImGuiKey_RightAlt;
			}
			case VK_RWIN: {
				return ImGuiKey_RightSuper;
			}
			case VK_APPS: {
				return ImGuiKey_Menu;
			}
			case '0': {
				return ImGuiKey_0;
			}
			case '1': {
				return ImGuiKey_1;
			}
			case '2': {
				return ImGuiKey_2;
			}
			case '3': {
				return ImGuiKey_3;
			}
			case '4': {
				return ImGuiKey_4;
			}
			case '5': {
				return ImGuiKey_5;
			}
			case '6': {
				return ImGuiKey_6;
			}
			case '7': {
				return ImGuiKey_7;
			}
			case '8': {
				return ImGuiKey_8;
			}
			case '9': {
				return ImGuiKey_9;
			}
			case 'A': {
				return ImGuiKey_A;
			}
			case 'B': {
				return ImGuiKey_B;
			}
			case 'C': {
				return ImGuiKey_C;
			}
			case 'D': {
				return ImGuiKey_D;
			}
			case 'E': {
				return ImGuiKey_E;
			}
			case 'F': {
				return ImGuiKey_F;
			}
			case 'G': {
				return ImGuiKey_G;
			}
			case 'H': {
				return ImGuiKey_H;
			}
			case 'I': {
				return ImGuiKey_I;
			}
			case 'J': {
				return ImGuiKey_J;
			}
			case 'K': {
				return ImGuiKey_K;
			}
			case 'L': {
				return ImGuiKey_L;
			}
			case 'M': {
				return ImGuiKey_M;
			}
			case 'N': {
				return ImGuiKey_N;
			}
			case 'O': {
				return ImGuiKey_O;
			}
			case 'P': {
				return ImGuiKey_P;
			}
			case 'Q': {
				return ImGuiKey_Q;
			}
			case 'R': {
				return ImGuiKey_R;
			}
			case 'S': {
				return ImGuiKey_S;
			}
			case 'T': {
				return ImGuiKey_T;
			}
			case 'U': {
				return ImGuiKey_U;
			}
			case 'V': {
				return ImGuiKey_V;
			}
			case 'W': {
				return ImGuiKey_W;
			}
			case 'X': {
				return ImGuiKey_X;
			}
			case 'Y': {
				return ImGuiKey_Y;
			}
			case 'Z': {
				return ImGuiKey_Z;
			}
			case VK_F1: {
				return ImGuiKey_F1;
			}
			case VK_F2: {
				return ImGuiKey_F2;
			}
			case VK_F3: {
				return ImGuiKey_F3;
			}
			case VK_F4: {
				return ImGuiKey_F4;
			}
			case VK_F5: {
				return ImGuiKey_F5;
			}
			case VK_F6: {
				return ImGuiKey_F6;
			}
			case VK_F7: {
				return ImGuiKey_F7;
			}
			case VK_F8: {
				return ImGuiKey_F8;
			}
			case VK_F9: {
				return ImGuiKey_F9;
			}
			case VK_F10: {
				return ImGuiKey_F10;
			}
			case VK_F11: {
				return ImGuiKey_F11;
			}
			case VK_F12: {
				return ImGuiKey_F12;
			}
			case VK_OEM_7: {  // Apostrophe
				return ImGuiKey_Apostrophe;
			}
			case VK_OEM_COMMA: {
				return ImGuiKey_Comma;
			}
			case VK_OEM_MINUS: {
				return ImGuiKey_Minus;
			}
			case VK_OEM_PERIOD: {
				return ImGuiKey_Period;
			}
			case VK_OEM_2: {  // Forward Slash
				return ImGuiKey_Slash;
			}
			case VK_OEM_1: {  // Semicolon
				return ImGuiKey_Semicolon;
			}
			case VK_OEM_PLUS: {
				return ImGuiKey_Equal;
			}
			case VK_OEM_4: {  // Left Bracket
				return ImGuiKey_LeftBracket;
			}
			case VK_OEM_5: {  // Backslash
				return ImGuiKey_Backslash;
			}
			case VK_OEM_6: {  // Right Bracket
				return ImGuiKey_RightBracket;
			}
			case VK_OEM_3: {  // Grave Accent
				return ImGuiKey_GraveAccent;
			}
			case VK_CAPITAL: {
				return ImGuiKey_CapsLock;
			}
			case VK_SCROLL: {
				return ImGuiKey_ScrollLock;
			}
			case VK_NUMLOCK: {
				return ImGuiKey_NumLock;
			}
			case VK_SNAPSHOT: {  // Print Screen
				return ImGuiKey_PrintScreen;
			}
			case VK_PAUSE: {
				return ImGuiKey_Pause;
			}
			case VK_NUMPAD0: {
				return ImGuiKey_Keypad0;
			}
			case VK_NUMPAD1: {
				return ImGuiKey_Keypad1;
			}
			case VK_NUMPAD2: {
				return ImGuiKey_Keypad2;
			}
			case VK_NUMPAD3: {
				return ImGuiKey_Keypad3;
			}
			case VK_NUMPAD4: {
				return ImGuiKey_Keypad4;
			}
			case VK_NUMPAD5: {
				return ImGuiKey_Keypad5;
			}
			case VK_NUMPAD6: {
				return ImGuiKey_Keypad6;
			}
			case VK_NUMPAD7: {
				return ImGuiKey_Keypad7;
			}
			case VK_NUMPAD8: {
				return ImGuiKey_Keypad8;
			}
			case VK_NUMPAD9: {
				return ImGuiKey_Keypad9;
			}
			case VK_DECIMAL: {
				return ImGuiKey_KeypadDecimal;
			}
			case VK_DIVIDE: {
				return ImGuiKey_KeypadDivide;
			}
			case VK_MULTIPLY: {
				return ImGuiKey_KeypadMultiply;
			}
			case VK_SUBTRACT: {
				return ImGuiKey_KeypadSubtract;
			}
			case VK_ADD: {
				return ImGuiKey_KeypadAdd;
			}
			case VK_RETURN + 256: {  // Numpad Enter (Special handling from ImGui)
				return ImGuiKey_KeypadEnter;
			}
			case VK_OEM_NEC_EQUAL: {  // Numpad Equal
				return ImGuiKey_KeypadEqual;
			}
			default: {
				return ImGuiKey_None;
			}
		}
	}
}
