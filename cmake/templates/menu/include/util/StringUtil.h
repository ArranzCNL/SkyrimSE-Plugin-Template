#pragma once

namespace UTIL::STRING {

	static inline std::string WideToUTF8(const std::wstring& wstr)
	{
		if (wstr.empty()) {
			return {};
		}

		std::int32_t sizeRequired = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, nullptr, 0, nullptr, nullptr);
		if (sizeRequired <= 0) {
			return {};
		}

		std::string result(sizeRequired, '\0');
		WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, result.data(), sizeRequired, nullptr, nullptr);

		result.pop_back();
		return result;
	}
}
