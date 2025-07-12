#pragma once

namespace Hook {

	namespace Function {

		// a_fadingOut:                      Fade game out or in.
		// a_blackFade:                      Black or white fade colour.
		// a_fadeInOutDuration:        Number of seconds to fade in/out the game.
		// a_holdFade:                       Hold fade when fading out is complete. Keeps game faded until a fade in call is made.
		// a_fadeHoldDuration:         Number of seconds to hold the fade before activating a_fadeInOutDuration.
		void FadeOutGame(bool a_fadingOut, bool a_blackFade, float a_fadeInOutDuration, bool a_holdFade, float a_fadeHoldDuration);
	}

	namespace Variable {

		extern const float* fDeltaWorldTime;
	}
}
