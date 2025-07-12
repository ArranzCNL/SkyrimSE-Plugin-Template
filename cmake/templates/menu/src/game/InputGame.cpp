#include "game/InputGame.h"

namespace Game {

	void RegisterInput::Register(Input* a_input)
	{
		m_inputs.emplace_back(a_input);
	}

	bool RegisterInput::Update(RE::InputEvent* const* a_event)
	{
		auto handle{ false };

		for (Game::Input* input : m_inputs) {

			if (input->ProcessInput(a_event)) {
				handle = true;
			}
		}
		return handle;
	}
}
