#pragma once

#include "RE/I/InputEvent.h"

namespace RE
{
	class CharEvent : public InputEvent
	{
	public:
		inline static constexpr auto RTTI = RTTI_CharEvent;
		inline static constexpr auto VTABLE = VTABLE_CharEvent;

		~CharEvent() override;  // 00

		void Init(std::uint32_t a_keyCode)
		{
			keycode = a_keyCode;
		}

		std::uint32_t keycode;  // 18
		std::uint32_t pad1C;    // 1C
	private:
		KEEP_FOR_RE()
	};
	static_assert(sizeof(CharEvent) == 0x20);
}
