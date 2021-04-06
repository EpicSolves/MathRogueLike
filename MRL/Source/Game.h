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
#include "Enemy.h"
#include <random>

#define DRAW_COLLISION_BOXES  false

#define MATH_PHASE_TIMER    5*60

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
	void addGates();
	void removeGates();
	void removeDeadEnemies();

	// Spawn a wave of enemies
	void spawnWave(Wave &wave);

	// Check for user hitting escape key
	void checkForQuitEvent();

	// Where we handle transitioning between rooms
	void handleLocationUpdates();

	// For handling how we transition to different phases of the game
	void handlePhaseUpdates();
	bool inMathPhase();

	// Update hood
	void updateHUD();
	void updateEnergy();
	void updateNameplates();

	// Update camera
	void updateCamera();

	static std::mt19937 rng;

	static Manager manager;
	static AssetManager *assets;
	static int counter;
	static bool isRunning;
	static SDL_Renderer *renderer;
	static SDL_Event event;
	static SDL_Rect camera;
	static bool hasKey;
	static int numberAVal;
	static int numberBVal;
	static const uint8_t *keyState;
	static bool healCD;
	static std::map<std::string, int> spelltimers;
	static int locationX;
	static int locationY;
	static Entity& player;
	static int enemyCount;
	static Hero hero;
	static bool updateLabels;
	static Map map;
	static std::vector<Enemy> enemyVec;
	static int roomsCleared;

	// Determines which phase of the game we are in
	static enum phase_enum { PEACE_PHASE, DOOR_PHASE, MATH_PHASE, FIGHT_PHASE, BOSS_PHASE };
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
		groupMapHazards,
		groupGear,
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
		groupExtraMapTiles,
		groupGates,
		groupNameplates
	};

private:
	SDL_Window *window;
};
#endif /* GAME_H */

