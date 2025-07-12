#pragma once

#include "menu/IMenu.h"

namespace Menu {

	class Player final : public IMenu {

	public:
		Player();
		virtual ~Player() = default;

		virtual void Open() override;
		virtual void Update() override;
		virtual void Close() override;
		virtual void Hotkey() override;
	};
}
