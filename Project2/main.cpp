#include "game.hpp"
Game* game = nullptr;

int main(int argc, char* argv[]) {
	const int FPS = 120;
	const int frameDelay = 1000 / FPS;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	Uint32 frameStart;
	int frameTime;
	game = new Game();
	game->init("TuxBall", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);
	while (game->running()) {

		frameStart = SDL_GetTicks64();	

		game->handleEvents();
		game->update();
		game->render(); 

		frameTime = SDL_GetTicks64() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();
	return 0;
}