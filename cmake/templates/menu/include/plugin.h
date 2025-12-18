#pragma once

#include "api/skse_api.h"

namespace DLLMAIN {

	struct PluginInfo {
		std::string name{};
		std::string description{};
		REL::Version version{};
		SKSE::PluginHandle handle{ 0 };
		std::string directory{};
		std::string gameDirectory{};
		REL::Version gameVersion{};
		std::uintptr_t gameBaseAddress{ 0 };
		std::uintptr_t gameImageSize{ 0 };
		REL::Version skseVersion{};
		bool loaded{ false };
	};

	class Plugin final : public REX::Singleton<Plugin> {

	public:
		Plugin();
		~Plugin();

	public:
		const PluginInfo& Info() const { return m_info; };

	private:
		bool Load(const SKSE::LoadInterface* a_skse);

	private:
		PluginInfo m_info{};

	private:
		friend bool SKSE::SKSEPlugin_Load(const SKSE::LoadInterface* a_skse);
	};
}
