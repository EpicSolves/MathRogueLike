#include "ClassList.h"
#include "Game.h"
class Manager;

Game *game = nullptr;

int main(int argc, char **argv) {
	
	const int FPS = 60;
	const int msPerS = 1000;
	const int frameDelay = 1000 / FPS; // Max time we have between frames in milliseconds
	
	Uint32 frameStart = 0;
	Uint32 frameEnd = 0;
	int frameTime;

	game = new Game();
	game->init("Arithmenaughts", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, false);

	while (game->running()) {
		
		// How many milliseconds since we initialized SDL
		frameStart = SDL_GetTicks();
		game->handleEvents();
		game->update();
		game->render();
		
		// Give us how much time it has taken to go through events and stuff
		frameTime = SDL_GetTicks() - frameStart;
		
		// Check to see if we need to delay rendering frame
		if (frameDelay > frameTime) {
			//printf("Delaying %d\n", frameDelay - frameTime);
			SDL_Delay(frameDelay - frameTime);
		}
		// frameEnd = SDL_GetTicks();
		// printf("%d\n", frameTime);
		// Uint32 ellapsedTime = (frameEnd - frameStart);
		// printf("%d\n", msPerS/ ellapsedTime);
	}

	game->clean();

	return 0;
}