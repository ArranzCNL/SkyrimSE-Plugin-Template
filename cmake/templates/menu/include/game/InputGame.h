#pragma once

namespace Hook::Input {
	struct ProcessInput;
}

namespace Game {

	class Input {

	public:
		Input() = default;
		virtual ~Input() = default;

		virtual bool ProcessInput(RE::InputEvent* const* a_event) = 0;
	};

	class RegisterInput final : public REX::Singleton<RegisterInput> {

	public:
		void Register(Input* a_input);

	private:
		bool Update(RE::InputEvent* const* a_event);

	private:
		std::vector<Input*> m_inputs;

		friend struct Hook::Input::ProcessInput;
	};
}
