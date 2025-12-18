#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include "ui/menu/fonts/IconsFontAwesome6Menu.h"
#include "util/KeyboardUtil.h"

namespace ImGui {

	void SetupContext(std::string& a_editorFile);
	void SetupBackend();
	void StartFrame();
	void FinalFrame();
	void ShutDown();

	const ImGuiKey VirtualKeyToImGuiKey(const std::uint32_t a_key);
}
