#pragma once

#include <spdlog/spdlog.h>

namespace Util {

	constexpr auto LOGGER_NAME = "Logger";
}

#if defined(_DEBUG)
	#define LOG_TRACE(...) if (spdlog::get(Util::LOGGER_NAME) != nullptr) spdlog::get(Util::LOGGER_NAME)->trace(__VA_ARGS__)
	#define LOG_DEBUG(...) if (spdlog::get(Util::LOGGER_NAME) != nullptr) spdlog::get(Util::LOGGER_NAME)->debug(__VA_ARGS__)
#else
	#define LOG_TRACE(...)
	#define LOG_DEBUG(...)
#endif

#define LOG_INFO(...) if (spdlog::get(Util::LOGGER_NAME) != nullptr) spdlog::get(Util::LOGGER_NAME)->info(__VA_ARGS__)
#define LOG_WARN(...) if (spdlog::get(Util::LOGGER_NAME) != nullptr) spdlog::get(Util::LOGGER_NAME)->warn(__VA_ARGS__)
#define LOG_ERROR(...) if (spdlog::get(Util::LOGGER_NAME) != nullptr) spdlog::get(Util::LOGGER_NAME)->error(__VA_ARGS__)
#define LOG_CRITICAL(...) if (spdlog::get(Util::LOGGER_NAME) != nullptr) spdlog::get(Util::LOGGER_NAME)->critical(__VA_ARGS__)
