#include <cstdlib>  // EXIT_SUCCESS
#include <iostream> // std::cerr

#include "game.hh"

#define FRAME_DELAY_MS 1000 / FPS_CAP

int main(int p_argc, char **p_argv) {
	// std::cerr is the logging stream on default
	// CityBuilder::LogInto(std::cerr);

	auto &game = CityBuilder::Game::Create(p_argc, p_argv);

	size_t fps_timer = 0, fps;
	while (not game.Quit()) {
		size_t start = SDL_GetTicks();
		size_t delta = start - fps_timer;

		fps       = 1000 / delta;
		fps_timer = start;

#ifdef CITY_BUILDER_DEBUG
		SDL_SetWindowTitle(game.window, (TITLE" | FPS: " + std::to_string(fps)).c_str());
#else
		UNUSED(fps);
#endif

		game.Render();
		game.Input();
		game.Update();

		size_t time = SDL_GetTicks() - start;
		if (FRAME_DELAY_MS > time)
			SDL_Delay(FRAME_DELAY_MS - time);
	}

	CityBuilder::Game::Delete();

	return EXIT_SUCCESS;
}
