#include "plugin.h"

#include "version.h"
#include "util/LogUtil.h"
#include "util/StringUtil.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <Psapi.h>

namespace DLLMAIN {

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Helper Functions
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr auto PLUGIN_PATH = "Data\\SKSE\\Plugins\\";
	static constexpr REL::Version SKSE_RUNTIME_2_0_18(2, 0, 18, 0);

	static std::string get_game_directory(bool a_pluginPath = false)
	{
		wchar_t currentDirectory[MAX_PATH]{};
		const DWORD directorySize = GetCurrentDirectoryW(MAX_PATH, currentDirectory);

		if (directorySize && directorySize <= MAX_PATH) {
			std::string path = UTIL::STRING::WideToUTF8(currentDirectory);
			if (a_pluginPath) {
				path += '\\';
				path += PLUGIN_PATH;
			}
			return path;
		}
		return {};
	}

	static void init_logging(const std::string& a_pluginName)
	{
		// Build log file
		auto logFile = std::format("{}\\{}.log", SKSE::log::log_directory()->string(), a_pluginName);
		// Setup spdlog
		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true);
		fileSink->set_pattern("%v");

#if defined(_DEBUG)
		// Create a console and file sink
		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_pattern("%^[%d/%m/%C %H:%M:%S] %v%$");
		std::vector<spdlog::sink_ptr> sinks{ fileSink, consoleSink };
#else
		// Create a file sink
		std::vector<spdlog::sink_ptr> sinks{ fileSink };
#endif
		// Register logger
		auto logger = std::make_shared<spdlog::logger>(UTIL::LOGGER_NAME, sinks.begin(), sinks.end());
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
		spdlog::register_logger(logger);

		// Set logging pattern
		fileSink->set_pattern("[%d/%m/%C %H:%M:%S] %v");
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	Plugin::Plugin()
	{
		m_info.name = VERSION_PRODUCTNAME_STR;
		m_info.description = VERSION_PRODUCTNAME_DESCRIPTION_STR;
		m_info.version = { VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD };
		m_info.directory = get_game_directory(true) + m_info.name + "\\";
		m_info.gameDirectory = get_game_directory();

		MODULEINFO skyrimInfo{};
		auto skyrim = GetModuleHandleW(L"SkyrimSE.exe");

		if (skyrim) {
			GetModuleInformation(GetCurrentProcess(), skyrim, &skyrimInfo, sizeof(MODULEINFO));
			m_info.gameBaseAddress = reinterpret_cast<std::uintptr_t>(skyrimInfo.lpBaseOfDll);
			m_info.gameImageSize = static_cast<std::uintptr_t>(skyrimInfo.SizeOfImage);
		}
	}

	Plugin::~Plugin()
	{
#if defined(_DEBUG)
		std::cout << "Debug Console Terminated." << std::endl;
		FreeConsole();
#endif
		// Shutdown spdlog
		spdlog::shutdown();
	}

	bool Plugin::Load(const SKSE::LoadInterface* a_skse)
	{
		// Initialize SKSE Interface
		SKSE::Init(a_skse, false);
		// Copy SkyrimSE version data
		m_info.gameVersion = a_skse->RuntimeVersion();
		// Get our plugin handle
		m_info.handle = SKSE::GetPluginHandle();
		// Initialize logging
		init_logging(m_info.name);
		// Log plugin initializing
		LOG_INFO("{} v{} initializing...", m_info.name, m_info.version.string("."));
		// Check SKSE runtime due to missing trampoline interface from 2.0.17 and below
		m_info.skseVersion = REL::Version::unpack(a_skse->SKSEVersion());
		if (m_info.skseVersion < SKSE_RUNTIME_2_0_18) {
			LOG_CRITICAL("Unsupported SKSE v{}", m_info.skseVersion.string("."));
			return false;
		}

		LOG_INFO("  SkyrimSE\tv{}", m_info.gameVersion.string("."));
		LOG_INFO("  SKSE\tv{}", m_info.skseVersion.string("."));
		// Install hooks

		return true;
	}
}
