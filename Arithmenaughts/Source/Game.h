#ifndef GAME_H
#define GAME_H
#include "SDL.h"
#include "SDL_image.h"
#include <cstdio>
#include <vector>
#include <map>

#define DRAW_COLLISION_BOXES  false

#define OVERWORLD_PHASE_TIMER  0*60
#define MATH_PHASE_0_TIMER    10*60
#define FIGHT_PHASE_0_TIMER   15*60
#define MATH_PHASE_1_TIMER    30*60
#define FIGHT_PHASE_1_TIMER   3*60*60
#define MATH_PHASE_2_TIMER    30*60
#define FIGHT_PHASE_2_TIMER   3*60*60
#define BOSS_PHASE_TIMER       0*60

class AssetManager;
class ColliderComponent;

class Game {

public:
	Game();
	~Game();

	void init(const char *title, int xPosition, int yPosition, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean() {
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}

	// Presents the user with an arithmetic problem
	void showSolvePad();
	void hideSolvePad();
	bool solvedProblem();
	void generatenewProblem();
	bool inSolvePadMode();

	// For removing tiles and colliders of the map
	void cleanMapTiles();
	void removeGate();

	static AssetManager *assets;
	static int counter;
	static bool isRunning;
	static SDL_Renderer *renderer;
	static SDL_Event event;
	static SDL_Rect camera;
	static bool eligibleA;
	static bool eligibleB;
	static bool hasKey;
	static int numberAVal;
	static int numberBVal;
	static const uint8_t *keyState;
	static bool healCD;
	static std::map<std::string, int> spelltimers;
	static int location;

	// Determines which phase of the game we are in
	static enum phase_enum { OVERWORLD_PHASE, MATH_PHASE_0, FIGHT_PHASE_0, 
	                                          MATH_PHASE_1, FIGHT_PHASE_1,
	                                          MATH_PHASE_2, FIGHT_PHASE_2, 
	                                         BOSS_PHASE };
	static int phase;
	static int phaseTimer;

	// Difficulty settings
	static int enemySpawnTimer;
	static int enemiesKilled;
	static int spawnRate;

	// Powerup settings for math phase
	static float shootSpeed;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupMisc,
		groupHUD,
		groupEnemyProjectiles,
		groupPlayerProjectiles,
		groupCollectibles,
		groupUILabels,
		groupMapColliders,
		groupExtraMapTiles
	};

private:
	SDL_Window *window;
};
#endif /* GAME_H */

