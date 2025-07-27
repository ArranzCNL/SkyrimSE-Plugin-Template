#pragma once

#include "RE/R/Renderer.h"

namespace PROXY {

	namespace BSGraphics {

		struct Renderer {

			static auto* Data(RE::BSGraphics::Renderer* a_renderer)
			{
#if defined(SKYRIM_SUPPORT_NG)
				return &a_renderer->GetRuntimeData();
#else
				return &a_renderer->data;
#endif
			}
		};
	}
}
