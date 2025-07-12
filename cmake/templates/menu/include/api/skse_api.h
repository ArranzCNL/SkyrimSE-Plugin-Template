#pragma once

#define SKSE_API extern "C" __declspec(dllexport)

namespace SKSE {

	SKSE_API bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse);

}
