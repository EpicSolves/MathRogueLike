#ifndef GAME_H
#define GAME_H
#include "TextureManager.h"
#include "ClassList.h"
#include "SDL.h"
#include "SDL_image.h"
#include <cstdio>
#include <vector>
#include <map>
#include "ECS.h"
#include "Wave.h"
#include "Hero.h"

#define DRAW_COLLISION_BOXES  false

#define MATH_PHASE_0_TIMER    10*60
#define MATH_PHASE_1_TIMER    10*60
#define MATH_PHASE_2_TIMER    10*60

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

	// Spawn a wave of enemies
	void spawnWave(Wave &wave);

	static Manager manager;
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
	static Entity& player;
	static int enemyCount;
	static Hero hero;

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

	// Spell and Ability cooldowns
	static int dashDuration;
	static bool dashCooldown;

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

