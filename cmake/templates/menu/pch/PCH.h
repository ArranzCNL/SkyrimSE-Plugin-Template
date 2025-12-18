#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <PROXY/Proxies.h>

#include <REX/REX/Singleton.h>

#include "util/HookUtil.h"

using namespace std::literals;

#if defined(SKYRIM_SUPPORT_NG)
	#define RELOCATION_OFFSET(a_se, a_ae) REL::VariantOffset(a_se, a_ae, 0x0).offset()
#else
	#if defined(SKYRIM_SUPPORT_AE)
		#define RELOCATION_OFFSET(se, ae) ae
	#else
		#define RELOCATION_OFFSET(se, ae) se
	#endif
#endif
