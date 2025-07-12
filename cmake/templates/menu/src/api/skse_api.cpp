#include "api/skse_api.h"

#include "plugin.h"
#include "version.h"
#include "util/LogUtil.h"

//#define DUMP_OFFSETS

#if defined(DUMP_OFFSETS)
#include "api/versionlibdb.h"

static void DumpOffsets(REL::Version a_gameVersion)
{
	VersionDb db;
	// Try to load database for current running executable version.
	if (!db.Load(a_gameVersion.major(), a_gameVersion.minor(), a_gameVersion.patch(), a_gameVersion.build())) {
		LOG_CRITICAL("Failed to load database for {}!", a_gameVersion.string("."));
		return;
	}

	// Dump offsets file to My Games\Skyrim Special Edition\SKSE directory
	auto dumpOffsets = std::format("{}\\{}-offsets.txt", SKSE::log::log_directory()->string(), a_gameVersion.string("."));
	db.Dump(dumpOffsets);
	LOG_INFO("Dumped offsets for SkyrimSE v{}", a_gameVersion.string("."));
}
#endif

namespace SKSE {

	static void SKSEPlugin_Message(SKSE::MessagingInterface::Message* a_message)
	{
		switch (a_message->type) {

			case SKSE::MessagingInterface::kPostPostLoad: {
				break;
			}
			case SKSE::MessagingInterface::kInputLoaded: {
				break;
			}
			case SKSE::MessagingInterface::kDataLoaded: {
				// Log plugin loaded
				LOG_INFO("{} loaded", DLLMain::Plugin::GetSingleton()->Info().name);
				break;
			}
		}
	}

#if defined(SKYRIM_SUPPORT_AE) || defined(SKYRIM_SUPPORT_NG)
	SKSE_API constinit auto SKSEPlugin_Version = []() noexcept {
		SKSE::PluginVersionData pluginData{};

		pluginData.PluginName(VERSION_PRODUCTNAME_STR);
		pluginData.PluginVersion(REL::Version(VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD));
		pluginData.AuthorName(VERSION_AUTHOR_STR);
		pluginData.AuthorEmail("https://github.com/ArranzCNL/ImprovedCameraSE-NG");
		pluginData.UsesAddressLibrary();
		pluginData.UsesNoStructs();

		return pluginData;
	}();
#endif

#if !defined(SKYRIM_SUPPORT_AE) || defined(SKYRIM_SUPPORT_NG)
	SKSE_API bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_pluginInfo)
	{
		a_pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
		a_pluginInfo->name = VERSION_PRODUCTNAME_STR;
		a_pluginInfo->version = REL::Version(VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD).pack();

		if (a_skse->IsEditor()) {
			LOG_CRITICAL("Loaded in editor, marking as incompatible");
			return false;
		}
		return true;
	}
#endif

	SKSE_API bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
	{
		// Load Plugin
		auto loadPlugin = DLLMain::Plugin::GetSingleton()->Load(a_skse);
		if (loadPlugin) {
			// Register SKSE::MessagingInterface
			SKSE::GetMessagingInterface()->RegisterListener(SKSEPlugin_Message);
		}
#if defined(DUMP_OFFSETS)
		DumpOffsets(a_skse->RuntimeVersion());
#endif
		return loadPlugin;
	}
}
