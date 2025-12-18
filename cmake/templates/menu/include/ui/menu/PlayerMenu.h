#pragma once

#include "ui/menu/IMenu.h"

namespace MENU {

	class PlayerMenu final : public IMenu {

	public:
		PlayerMenu();
		virtual ~PlayerMenu() = default;

		virtual void Open() override;
		virtual void Update() override;
		virtual void Close() override;
		virtual void Hotkey() override;
	};
}
