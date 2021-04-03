#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "StatsComponent.h"
#include <cmath>
#include <ctime>
#include "SDL_ttf.h"
#include "ECS.h"
#include "UILabel.h"

#define RADTODEG(R)((180.0 * R) / PI)
#define PI 3.14159
#define SDL_SUCCESS 0
#define MAP_SCALE 4
#define TILE_SIZE 32

Map Game::map;
Manager Game::manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

bool Game::isRunning = false;
bool Game::hasKey = false;
bool Game::updateLabels = false;
int Game::enemiesKilled = 0;
SDL_Rect Game::camera = { 0, 0, 1024, 768 };
AssetManager* Game::assets = new AssetManager();

int Game::numberAVal = 0;
int Game::numberBVal = 0;
int Game::counter = 0;
int Game::enemyCount = 0;

// Initialize phase to math phase and timer to 15 seconds
int Game::phase = Game::OVERWORLD_PHASE;
int Game::phaseTimer = MATH_PHASE_0_TIMER;
int Game::locationX = 0;
int Game::locationY = 0;

// Difficulty settings
int Game::enemySpawnTimer = 0;
int Game::spawnRate = 60 * 5;
std::map<std::string, int> Game::spelltimers = std::map<std::string, int>();

// Shoot speed based on initial math settings
float Game::shootSpeed = 5.0f;
const uint8_t *Game::keyState = 0;

// Spell and ability cooldowns
int Game::dashDuration = 0;
bool Game::dashCooldown = false;
 
//int Map::mapWidth = TILE_SIZE * MAP_SCALE * 10;
//int Map::mapHeight = TILE_SIZE * MAP_SCALE * 10;

// create a list of objects to be used in our game
Hero Game::hero = Hero();
auto& energy(Game::manager.addEntity());
auto& health(Game::manager.addEntity());
auto& numberA(Game::manager.addEntity());
auto& numberB(Game::manager.addEntity());

// Create a list of label objects
auto& healthLabel(Game::manager.addEntity());
auto& energyLabel(Game::manager.addEntity());
auto& score(Game::manager.addEntity());
auto& strengthLabel(Game::manager.addEntity());

// Create out groups
auto& players(Game::manager.getGroup(Game::groupPlayers));
auto& miscs(Game::manager.getGroup(Game::groupMisc));
auto& enemies(Game::manager.getGroup(Game::groupEnemies));
auto& headsUpDisplay(Game::manager.getGroup(Game::groupHUD));
auto& enemyProjectiles(Game::manager.getGroup(Game::groupEnemyProjectiles));
auto& playerProjectiles(Game::manager.getGroup(Game::groupPlayerProjectiles));
auto& collectibles(Game::manager.getGroup(Game::groupCollectibles));
auto& colliders(Game::manager.getGroup(Game::groupColliders));
auto& UILabels(Game::manager.getGroup(Game::groupUILabels));
auto& mapTiles(Game::manager.getGroup(Game::groupMap));
auto& mapColliders(Game::manager.getGroup(Game::groupMapColliders));
auto& extraMapTiles(Game::manager.getGroup(Game::groupExtraMapTiles));
auto& gear(Game::manager.getGroup(Game::groupGear));

// Game constructor/deconstructor defaults
Game::Game() {}
Game::~Game() {}

/**************************************************************************/
/*                                                                        */
/* Initialization code                                                    */
/*                                                                        */
/**************************************************************************/
void Game::init(const char *title, int xPosition, int yPosition, int width, int height, bool fullscreen) {

	// Seed the RNG
	srand(time(NULL));

	// Handle flags, such as "fullscreen"
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	// Initialize the subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != SDL_SUCCESS) {
		printf("Error initializing subsystems.\n");
		this->isRunning = false;
		return;
	}

	if (TTF_Init() != SDL_SUCCESS) {
		this->isRunning = false;
		return;
	}

	// Initialize the window
	this->window = SDL_CreateWindow(title, xPosition, yPosition, width, height, flags);

	// Check to ensure the window is open
	if (!window) {
		printf("Failed to create window.\n");
		this->isRunning = false;
		return;
	}

	this->renderer = SDL_CreateRenderer(this->window, -1, flags);
	if (!renderer) {
		printf("Failed to create renderer.\n");
		this->isRunning = false;
		return;
	}

	// Set render draw color
	SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);

	// Everything has worked so far
	isRunning = true;
	
	// Load textures
	assets->AddTexture("NewtonsGroveOverworld_Tileset", "Assets/Textures/NewtonsGroveOverworld_Tileset.png");
	assets->AddTexture("NewtonsGroveD0_Tileset", "Assets/Textures/NewtonsGroveD0_Tileset.png");
	assets->AddTexture("player_run", "Assets/Textures/player_run.png");
	assets->AddTexture("player_dash", "Assets/Textures/player_dash.png");
	assets->AddTexture("numbers", "Assets/Textures/numbers.png");
	assets->AddTexture("star_spin", "Assets/Textures/star_spin.png");
	assets->AddTexture("energy", "Assets/Textures/energy.png");
	assets->AddTexture("health", "Assets/Textures/health.png");
	assets->AddTexture("spell_heal", "Assets/Textures/spell_heal.png");
	assets->AddTexture("skeleton_idle", "Assets/Textures/skeleton_idle.png");
	assets->AddTexture("key", "Assets/Textures/key.png");
	assets->AddTexture("gold_bow", "Assets/Textures/gold_bow.png");
	assets->AddTexture("player_idle_base", "Assets/Textures/PlayerIdleBase.png");
	assets->AddTexture("player_idle_bow_brown", "Assets/Textures/PlayerIdleBowBrown.png");
	assets->AddTexture("player_idle_bow_purple", "Assets/Textures/PlayerIdleBowPurple.png");
	assets->AddTexture("player_run_base", "Assets/Textures/PlayerRunBase.png");
	assets->AddTexture("player_run_bow_brown", "Assets/Textures/PlayerRunBrown.png");
	assets->AddTexture("player_run_bow_purple", "Assets/Textures/PlayerRunPurple.png");

	// Load fonts
	assets->AddFont("verdana", "Assets/Fonts/Verdana.ttf", 20);

	// Load sounds
	assets->AddSound("star_shot", "Assets/Sounds/star.wav");
	assets->AddSound("enemy_hit", "Assets/Sounds/enemy_hit.wav");
	assets->AddSound("heal_cast", "Assets/Sounds/heal.wav");
	assets->AddSound("player_hit", "Assets/Sounds/player_hit.wav");
	assets->AddSound("game_over", "Assets/Sounds/game_over.wav");
	assets->AddSound("energy_up", "Assets/Sounds/energy_up.wav");
	assets->AddSound("skeleton_dead", "Assets/Sounds/skeleton_dead.wav");
	assets->AddSound("boss_dead", "Assets/Sounds/victory.wav");
	
	// Load the map
	map = Map("NewtonsGroveOverworld_Tileset", MAP_SCALE, TILE_SIZE);
	map.generateGraph(2, Vector2D(0,0), Vector2D(5, 3));
	map.generateRooms("NewtonsGroveD0_Tileset", 10, 10);
	map.LoadMap(map.graphRoot->mapName, map.roomWidth, map.roomHeight, map.texId);

	// Get an initial keyboard state
	Game::keyState = SDL_GetKeyboardState(NULL);

	// Create Labels
	SDL_Color white = { 255, 255, 255, 200 };
	healthLabel.addComponent<UILabel>(10, 10, "Health", "verdana", white, "health_label");
	healthLabel.addGroup(groupUILabels);
	energyLabel.addComponent<UILabel>(10, 50, "Energy", "verdana", white, "energy_label");
	energyLabel.addGroup(groupUILabels);
	score.addComponent<UILabel>(10, 90, "Score: 0", "verdana", white, "score_label");
	score.addGroup(groupUILabels);

	// Add components to our player
	Game::hero.init();
	Game::hero.setBow("brown");

	// Add health bar display
	health.addComponent<TransformComponent>(10.0f, 10.0f, 100.0f, 10.0f, 4);
	health.addComponent<SpriteComponent>("health");
	health.addComponent<StatsComponent>("health", 100.0f);
	health.addGroup(groupHUD);

	// Add energy bar display
	energy.addComponent<TransformComponent>(10.0f, 55.0f, 100.0f, 10.0f, 4);
	energy.addComponent<SpriteComponent>("energy");
	energy.addGroup(groupHUD);

	// The first digit of the number pop-up interface
	numberA.addComponent<TransformComponent>(300, 800, 32, 32, 4);
	numberA.addComponent<SpriteComponent>("numbers");
	numberA.getComponent<SpriteComponent>().sRect.x = 0;
	numberA.getComponent<SpriteComponent>().sRect.y = 0;
	numberA.getComponent<SpriteComponent>().sRect.w = 32;
	numberA.addGroup(groupHUD);

	// The second digit of the number pop-up interface
	numberB.addComponent<TransformComponent>(400, 800, 32, 32, 4);
	numberB.addComponent<SpriteComponent>("numbers");
	numberB.getComponent<SpriteComponent>().sRect.x = 0;
	numberB.getComponent<SpriteComponent>().sRect.y = 0;
	numberB.getComponent<SpriteComponent>().sRect.w = 32;
	numberB.addGroup(groupHUD);
}

/**************************************************************************/
/*                                                                        */
/* Handle events                                                          */
/*                                                                        */
/* Poll for events and then handle. This happens before we update stuff.  */
/*                                                                        */
/**************************************************************************/
void Game::handleEvents() {
	
	// Poll our static keyboard input
	SDL_PollEvent(&Game::event);

	// Get keyboard state
	Game::keyState = SDL_GetKeyboardState(NULL);
}

/**************************************************************************/
/*                                                                        */
/* Update objects based on what happened with the events.                 */
/*                                                                        */
/**************************************************************************/
void Game::update() {

	// See if we should exit the game
	checkForQuitEvent();

	// For handling things when we enter a new room
	handleLocationUpdates();

	// For handling when how we transition phases and what we do when we do transition
	handlePhaseUpdates();

	// If we are in the math phase
	if (inMathPhase()) {

		// Show solve pad and check to see if we have solved a problem. Increment stats if so
		showSolvePad();
		if (solvedProblem()) {
			Game::assets->PlaySound("energy_up");
			Game::hero.solveAgilityBonus += (rand() % 100) * 0.0002f;
			Game::hero.solveStrengthBonus += (rand() % 100) * 0.0002f;
			Game::hero.solveStaminaBonus += (rand() % 100) * 0.0002f;
			Game::hero.solveCriticalBonus += (rand() % 100) * 0.0002f;
			Game::hero.statsChanged = true;
			generatenewProblem();
		}
	}
	else {
		hideSolvePad();
	}

	// Keep record of players current position to handle collision after the update
	Vector2D playerPos = Game::hero.transform->position;

	// Have to make sure bow is in lock-step with player
	//Game::hero.bowTransform->position.x = playerPos.x + 5;
	Game::hero.bowSprite->currentFrame = Game::hero.sprite->currentFrame;
	Game::hero.bowSprite->spriteFlip = Game::hero.sprite->spriteFlip;

	// Check if player hit enemy
	for (auto& p : playerProjectiles) {
		for (auto& e : enemies) {
			if (Collision::AABB(e->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
				p->destroy();

				if (Game::hero.didCrit())
					e->getComponent<ColliderComponent>().health -= Game::hero.damagerPerShot*1.5f;
				else
					e->getComponent<ColliderComponent>().health -= Game::hero.damagerPerShot;

				if (e->getComponent<ColliderComponent>().health <= 0.0f && e->isActive()) {

					// Enemy was killed
					e->destroy();
					Game::enemyCount--;
					if (Game::enemyCount == 0 && Game::phase == BOSS_PHASE) {
						Entity &loot = Game::manager.addEntity();
						loot.addComponent<TransformComponent>(e->getComponent<ColliderComponent>().collider.x,
							e->getComponent<ColliderComponent>().collider.y, 128, 128, 2);
						loot.addComponent<SpriteComponent>("player_idle_bow_purple");
						loot.addComponent<ColliderComponent>("player_idle_bow_purple", 30, 30, 60, 60, false);
						loot.addGroup(groupCollectibles);
					}

					// increment score
					Game::enemiesKilled++;
					for (auto& l : UILabels) {
						if (l->getComponent<UILabel>().tagId == "score_label")
							l->destroy();
					}

					auto& newScore(Game::manager.addEntity());
					SDL_Color white = { 255, 255, 255, 200 };
					std::string scoreStr = "Score: " + std::to_string(Game::enemiesKilled);
					newScore.addComponent<UILabel>(10, 90, scoreStr, "verdana", white, "score_label");
					newScore.addGroup(groupUILabels);

					Game::assets->PlaySound("skeleton_dead");
				}
				else {
					Game::assets->PlaySound("enemy_hit");
				}
			}
		}
	}

	Game::manager.refresh();
	Game::manager.update();

	SDL_Rect playerCol = Game::hero.collider->collider;
	for (auto& c : mapColliders) {
		
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		// If there was a collision, see which bound collider
		// Check if top of player hit bottom of box
		if (Collision::ABB(Vector2D(playerCol.x + 10, playerCol.y), cCol)) {
			Game::hero.transform->position.y = playerPos.y + 0.01f;
			Game::hero.collider->update();
			playerCol = Game::hero.collider->collider;

			if (Game::hasKey) {
				if (c->getComponent<ColliderComponent>().tag == "GateA" || c->getComponent<ColliderComponent>().tag == "GateB")
					removeGate();
			}
		}

		// Check if left of player hit right of box
		if (Collision::ABB(Vector2D(playerCol.x, playerCol.y+10), cCol)) {
			Game::hero.transform->position.x = playerPos.x + 0.01f;
			Game::hero.collider->update();
			playerCol = Game::hero.collider->collider;

			if (Game::hasKey) {
				if (c->getComponent<ColliderComponent>().tag == "GateA" || c->getComponent<ColliderComponent>().tag == "GateB")
					removeGate();
			}
		}

		// Check if right of player hit left of box
		if (Collision::ABB(Vector2D(playerCol.x + playerCol.w, playerCol.y + 10), cCol)) {
			Game::hero.transform->position.x = playerPos.x - 0.01f;
			Game::hero.collider->update();
			playerCol = Game::hero.collider->collider;

			if (Game::hasKey) {
				if (c->getComponent<ColliderComponent>().tag == "GateA" || c->getComponent<ColliderComponent>().tag == "GateB")
					removeGate();
			}
		}

		// Check if bottom of player hit top of box
		if (Collision::ABB(Vector2D(playerCol.x + 10, playerCol.y + playerCol.h), cCol)) {
			Game::hero.transform->position.y = playerPos.y - 0.01f;
			Game::hero.collider->update();
			playerCol = Game::hero.collider->collider;

			if (Game::hasKey) {
				if (c->getComponent<ColliderComponent>().tag == "GateA" || c->getComponent<ColliderComponent>().tag == "GateB")
					removeGate();
			}
		}
	}
	
	// If enemy hit player
	for (auto& p : enemyProjectiles) {
		
		if (Collision::AABB(Game::hero.collider->collider, p->getComponent<ColliderComponent>().collider)) {
			p->destroy();
			Game::assets->PlaySound("player_hit");
			float hVal = health.getComponent<StatsComponent>().getResource("health");
			health.getComponent<StatsComponent>().setResource("health", hVal - Game::hero.damageReductionCoeff*20.0f);
			if (health.getComponent<StatsComponent>().getResource("health") <= 0) {
				Game::assets->PlaySound("game_over");
				Game::hero.transform->position.x = 800*MAP_SCALE;
				Game::hero.transform->position.y = 600*MAP_SCALE;
				health.getComponent<StatsComponent>().setResource("health", 100.0f);
				SDL_Delay(5000);
				Game::isRunning = false;
			}
		}
	}

	// If player hits collectible
	for (auto& c : collectibles) {
		if (Collision::AABB(Game::hero.collider->collider, c->getComponent<ColliderComponent>().collider)) {
			for (auto& g : gear) 
				g->destroy();
			Game::hasKey = true;
			Game::hero.setBow("purple");
			Game::hero.bowAgility = 5;
			Game::hero.bowStrength = 5;
			Game::hero.bowCritical = 5;
			Game::hero.bowStamina = 5;
			Game::hero.statsChanged = true;
			c->destroy();
		}
	}
	
	camera.x = std::max(0.0f, playerPos.x - 400);
	camera.y = std::max(0.0f, playerPos.y - 320);
	camera.x = std::min(camera.x, Game::map.mapWidth - camera.w);
	camera.y = std::min(camera.y, Game::map.mapHeight - camera.h);
	
	// Keep health bar in top left of view
	health.getComponent<TransformComponent>().position.x = camera.x + 10.0;
	health.getComponent<TransformComponent>().position.y = camera.y + 10.0;

	// Keep energy bar in top left of view
	energy.getComponent<TransformComponent>().position.x = camera.x + 10.0;
	energy.getComponent<TransformComponent>().position.y = camera.y + 50.0;
	
	// Refill some energy
	updateEnergy();

	// Update HUD (update hit points and energy bars)
	updateHUD();

	if (Game::updateLabels) {

		// Create new stats labels
		char labelBuffer[32];
		SDL_Color white = { 255, 255, 255, 75 };

		auto& staLabel(Game::manager.addEntity());
		auto& strLabel(Game::manager.addEntity());
		auto& agiLabel(Game::manager.addEntity());
		auto& critLabel(Game::manager.addEntity());


		snprintf(labelBuffer, 32, "Strength: %02.2f", Game::hero.strength + Game::hero.bowStrength + Game::hero.solveStrengthBonus);
		strLabel.addComponent<UILabel>(10, camera.h - 125, std::string(labelBuffer), "verdana", white, "a");

		snprintf(labelBuffer, 32, "Agility: %02.2f", Game::hero.agility + Game::hero.bowAgility + Game::hero.solveAgilityBonus);
		agiLabel.addComponent<UILabel>(10, camera.h - 150, std::string(labelBuffer), "verdana", white, "b");

		snprintf(labelBuffer, 32, "Stamina: %02.2f", Game::hero.stamina + Game::hero.bowStamina + Game::hero.solveStaminaBonus);
		staLabel.addComponent<UILabel>(10, camera.h - 175, std::string(labelBuffer), "verdana", white, "c");

		snprintf(labelBuffer, 32, "Critical: %02.2f%%", Game::hero.critical + Game::hero.bowCritical + Game::hero.solveCriticalBonus);
		critLabel.addComponent<UILabel>(10, camera.h - 200, std::string(labelBuffer), "verdana", white, "d");

		strLabel.addGroup(groupUILabels);
		agiLabel.addGroup(groupUILabels);
		staLabel.addGroup(groupUILabels);
		critLabel.addGroup(groupUILabels);
		Game::updateLabels = false;
	}

	// Remake labels
	if (Game::hero.statsChanged) {

		// Handle stat updates
		Game::hero.calculateStats();

		// Destroy current stats labels
		for (auto& l : UILabels) {
			if (l->getComponent<UILabel>().tagId == "a" ||
				l->getComponent<UILabel>().tagId == "b" ||
				l->getComponent<UILabel>().tagId == "c" ||
				l->getComponent<UILabel>().tagId == "d") {
				l->destroy();
			}
		}
		Game::updateLabels = true;
	}
}

/**************************************************************************/
/*                                                                        */
/* Draw the new stuff on the screen.                                      */
/*                                                                        */
/**************************************************************************/
void Game::render() {

	// Before we render, lock step animations
	SDL_RenderClear(renderer);
	for (auto& t : mapTiles) t->draw();
	for (auto& t : extraMapTiles) t->draw();
	for (auto& c : colliders) c->draw();
	for (auto& p : players) p->draw();
	for (auto& g : gear) g->draw();
	for (auto& m : miscs) m->draw();
	for (auto& e : enemies) e->draw();
	for (auto& c : collectibles) c->draw();
	for (auto& p : playerProjectiles) p->draw();
	for (auto& p : enemyProjectiles) p->draw();
	for (auto& h : headsUpDisplay) h->draw();
	for (auto& l : UILabels) l->draw();
	SDL_RenderPresent(renderer);
}

/**************************************************************************/
/*                                                                        */
/* Functions to clean up game loop.                                       */
/*                                                                        */
/**************************************************************************/
void Game::showSolvePad() {
	numberA.getComponent<SpriteComponent>().sRect.x = 32 * Game::numberAVal;
	numberB.getComponent<SpriteComponent>().sRect.x = 32 * Game::numberBVal;
	numberA.getComponent<TransformComponent>().position.x = camera.x + camera.w/2 - 50 - 16;
	numberB.getComponent<TransformComponent>().position.x = camera.x + camera.w/2 + 50;
	numberA.getComponent<TransformComponent>().position.y = camera.y + camera.h - 150;
	numberB.getComponent<TransformComponent>().position.y = camera.y + camera.h - 150;
}

void Game::hideSolvePad() {
	numberA.getComponent<TransformComponent>().position.x = -200;
	numberB.getComponent<TransformComponent>().position.x = -200;
}

bool Game::solvedProblem() {
	if (Game::keyState[SDL_SCANCODE_KP_1 + Game::numberAVal + Game::numberBVal - 1] || Game::keyState[SDL_SCANCODE_KP_0 + Game::numberAVal + Game::numberBVal])
		return true;
	if (Game::keyState[SDL_SCANCODE_1 + Game::numberAVal + Game::numberBVal - 1] || Game::keyState[SDL_SCANCODE_0 + Game::numberAVal + Game::numberBVal])
		return true;
	return false;
}

void Game::generatenewProblem() {
	int currentAnswer = Game::numberAVal + Game::numberBVal;

	// Keep generating numbers until we have a new addition problem
	while (currentAnswer == Game::numberAVal + Game::numberBVal) {
		Game::numberAVal = rand() % 5;
		Game::numberBVal = rand() % 5;
	}
}

bool Game::inSolvePadMode() {
	return Game::keyState[SDL_SCANCODE_LCTRL];
}

void Game::cleanMapTiles() {
	for (auto& tile : mapTiles)
		tile->destroy();
	for (auto& tileCollider : mapColliders)
		tileCollider->destroy();
}

void Game::removeGate() {
	for (auto& tile : mapColliders) {
		if (tile->getComponent<ColliderComponent>().tag == "GateA" ||
			tile->getComponent<ColliderComponent>().tag == "GateB")
			tile->destroy();
	}
	for (auto& tile : extraMapTiles) {
		tile->destroy();
	}
}

void Game::spawnWave(Wave &wave) {
	// Spawn enemies from right
	for (int i = 0; i < wave.rightEnemies; i++)
		Game::assets->SpawnEnemy(Vector2D(11 * 32 * MAP_SCALE, rand() % (10 * 32 * MAP_SCALE)), wave.scale, wave.health, wave.canShoot, "skeleton_idle");

	// Spawn enemies from the left
	for (int i = 0; i < wave.leftEnemies; i++)
		Game::assets->SpawnEnemy(Vector2D(-1 * 32 * MAP_SCALE, rand() % (10 * 32 * MAP_SCALE)), wave.scale, wave.health, wave.canShoot, "skeleton_idle");

	// Spawn from top
	for (int i = 0; i < wave.topEnemies; i++)
		Game::assets->SpawnEnemy(Vector2D(rand() % (10 * 32 * MAP_SCALE), -1*32*MAP_SCALE), wave.scale, wave.health, wave.canShoot, "skeleton_idle");

	// Spawn from bottom
	for (int i = 0; i < wave.bottomEnemies; i++)
		Game::assets->SpawnEnemy(Vector2D(rand() % (10 * 32 * MAP_SCALE), 11 * 32 * MAP_SCALE), wave.scale, wave.health, wave.canShoot, "skeleton_idle");
}

void Game::checkForQuitEvent() {
	this->isRunning = Game::event.type != SDL_QUIT;
}
/*
void Game::handleLocationUpdates() {
	
	if (Game::location == 0) {
		// Check if the user entered a dungeon

		if (Game::hero.collider->collider.x > Map::mapWidth) {
			Game::location = 1;

			// Clean it up
			// free(map);
			cleanMapTiles();

			// Load dungeon 0
			Map::mapWidth = TILE_SIZE * MAP_SCALE * 100;
			Map::mapHeight = TILE_SIZE * MAP_SCALE * 100;
			// map = new Map("NewtonsGroveD0_Tileset", MAP_SCALE, TILE_SIZE);
			map->setTileset("NewtonsGroveD0_Tileset");
			map->generateOverworld("Assets/Maps/overworld0.map", 100, 100);
			map->LoadMap("Assets/Maps/overworld0.map", 100, 100, "NewtonsGroveD0_Tileset");
			Game::hero.transform->position.x = 64;
			Game::hero.collider->update();

			// Shut the gate
			/*
			auto& newTile(Game::manager.addEntity());
			newTile.addComponent<TransformComponent>(0, 6 * TILE_SIZE* MAP_SCALE, TILE_SIZE, TILE_SIZE, MAP_SCALE);
			newTile.addComponent<SpriteComponent>("NewtonsGroveD0_Tileset");

			newTile.getComponent<SpriteComponent>().sRect.x = 9 * 32;
			newTile.getComponent<SpriteComponent>().sRect.y = 32;
			newTile.addGroup(groupExtraMapTiles);

			auto& gateACollider(Game::manager.addEntity());
			gateACollider.addComponent<ColliderComponent>("GateA", 0, 6 * TILE_SIZE*MAP_SCALE, 32 * MAP_SCALE, true);
			gateACollider.addGroup(groupMapColliders);

			// Shut the gate
			auto& newTile2(Game::manager.addEntity());
			newTile2.addComponent<TransformComponent>(0, 7 * TILE_SIZE* MAP_SCALE, TILE_SIZE, TILE_SIZE, MAP_SCALE);
			newTile2.addComponent<SpriteComponent>("NewtonsGroveD0_Tileset");
			newTile2.getComponent<SpriteComponent>().sRect.x = 0 * 32;
			newTile2.getComponent<SpriteComponent>().sRect.y = 2 * 32;
			// newTile2.addComponent<ColliderComponent>("GateB", 0, 7 * TILE_SIZE* MAP_SCALE, 32 * MAP_SCALE, false);
			newTile2.addGroup(groupExtraMapTiles);

			auto& gateBCollider(Game::manager.addEntity());
			gateBCollider.addComponent<ColliderComponent>("GateB", 0, 7 * TILE_SIZE*MAP_SCALE, 32 * MAP_SCALE, true);
			gateBCollider.addGroup(groupMapColliders);
		}
	}
	else if (Game::location == 1) {
		/*
		if (Game::hero.collider->collider.x < -50) {
			// Clean it up
			free(map);
			cleanMapTiles();

			// Load dungeon 0
			Map::mapWidth = TILE_SIZE * MAP_SCALE * 10;
			Map::mapHeight = TILE_SIZE * MAP_SCALE * 10;
			map = new Map("NewtonsGroveOverworld_Tileset", MAP_SCALE, TILE_SIZE);
			map->LoadMap("Assets/Maps/NewtonsGroveOverworld.map", 10, 10, "NewtonsGroveOverworld_Tileset");
			Game::hero.transform->position.x = 1000;
			Game::hero.collider->update();
			Game::location = 0;
		}
		
		//printf("%d\n", map->cavePositionsX[0]);
		// Check to see if we have entered a case
		for (int i = 0; i < 10; i++) {
			//printf("%d %d %d %d\n", Game::hero.collider->collider.x, Game::hero.collider->collider.y, map->cavePositionsX[i]*TILE_SIZE*MAP_SCALE, map->cavePositionsY[i]*TILE_SIZE*MAP_SCALE);
			if (Game::hero.collider->collider.x > map->cavePositionsX[i]*TILE_SIZE*MAP_SCALE && 
				Game::hero.collider->collider.x < (map->cavePositionsX[i] + 2) * TILE_SIZE*MAP_SCALE  &&
				Game::hero.collider->collider.y > map->cavePositionsY[i] *TILE_SIZE*MAP_SCALE &&
				Game::hero.collider->collider.y < (map->cavePositionsY[i] + 2) * TILE_SIZE*MAP_SCALE) {
				Game::location = 2;
				Game::phase = Game::MATH_PHASE_0;
				Game::phaseTimer = MATH_PHASE_0_TIMER;

				// Clean it up
				cleanMapTiles(); // Remove all current map tiles

				// Load dungeon 0
				Map::mapWidth = TILE_SIZE * MAP_SCALE * 10;
				Map::mapHeight = TILE_SIZE * MAP_SCALE * 10;
				// map = new Map("NewtonsGroveD0_Tileset", MAP_SCALE, TILE_SIZE);
				map->setTileset("NewtonsGroveD0_Tileset");
				map->generateRoom("Assets/Maps/room0.map", 10, 10);
				map->LoadMap("Assets/Maps/room0.map", 10, 10, "NewtonsGroveD0_Tileset");
				Game::hero.transform->position.x = 300;
				Game::hero.transform->position.y = 300;
				Game::hero.collider->update();
				break;
			}
		}
		
	} 
}*/

void Game::handleLocationUpdates() {
	if (Game::hero.collider->collider.x > Game::map.mapWidth) {
		Game::locationX++;
		std::string nextRoom = "Assets/Maps/Room" + std::to_string(Game::locationX) + "." +
			std::to_string(Game::locationY) + ".map";
		cleanMapTiles();
		map.LoadMap(nextRoom, map.roomWidth, map.roomHeight, map.texId);
		Game::hero.transform->position.x = -Game::hero.collider->xOffset;
		Game::hero.collider->update();
	} else 	if (Game::hero.collider->collider.x < -Game::hero.collider->collider.w) {
		Game::locationX--;
		std::string nextRoom = "Assets/Maps/Room" + std::to_string(Game::locationX) + "." +
			std::to_string(Game::locationY) + ".map";
		cleanMapTiles();
		map.LoadMap(nextRoom, map.roomWidth, map.roomHeight, map.texId);
		Game::hero.transform->position.x = Game::map.mapWidth - Game::hero.collider->collider.w - Game::hero.collider->xOffset-1.0f;
		Game::hero.collider->update();
	} else 	if (Game::hero.collider->collider.y > Game::map.mapHeight) {
		Game::locationY++;
		std::string nextRoom = "Assets/Maps/Room" + std::to_string(Game::locationX) + "." +
			std::to_string(Game::locationY) + ".map";
		cleanMapTiles();
		map.LoadMap(nextRoom, map.roomWidth, map.roomHeight, map.texId);
		Game::hero.transform->position.y = -Game::hero.collider->yOffset;
		Game::hero.collider->update();
	} else 	if (Game::hero.collider->collider.y < -Game::hero.collider->collider.h) {
		Game::locationY--;
		std::string nextRoom = "Assets/Maps/Room" + std::to_string(Game::locationX) + "." +
			std::to_string(Game::locationY) + ".map";
		cleanMapTiles();
		map.LoadMap(nextRoom, map.roomWidth, map.roomHeight, map.texId);
		Game::hero.transform->position.y = Game::map.mapHeight - Game::hero.collider->collider.h;
		Game::hero.collider->update();
	}
}

void Game::handlePhaseUpdates() {
	if (Game::phase == Game::MATH_PHASE_0) {
		Game::phaseTimer--;
		if (Game::phaseTimer <= 0) {
			Game::phase = Game::FIGHT_PHASE_0;

			// Create wave
			Wave w = Wave(0, 0, 3, 3, 2, 100.0f, true);
			spawnWave(w);
		}
	}
	else if (Game::phase == Game::FIGHT_PHASE_0) {

		if (Game::enemyCount == 0) {
			Game::phase = Game::MATH_PHASE_1;
			Game::phaseTimer = MATH_PHASE_1_TIMER;
		}
	}
	else if (Game::phase == Game::MATH_PHASE_1) {
		Game::phaseTimer--;
		if (Game::phaseTimer <= 0) {
			Game::phase = Game::FIGHT_PHASE_1;

			// 2 enemies per side
			Wave w = Wave(3, 3, 0, 0, 2, 100.0f, true);
			spawnWave(w);
		}
	}
	else if (Game::phase == Game::FIGHT_PHASE_1) {

		if (Game::enemyCount == 0) {
			Game::phase = Game::MATH_PHASE_2;
			Game::phaseTimer = MATH_PHASE_2_TIMER;
		}
	}
	else if (Game::phase == Game::MATH_PHASE_2) {
		Game::phaseTimer--;
		if (Game::phaseTimer <= 0) {
			Game::phase = Game::FIGHT_PHASE_2;
			Wave w = Wave(3, 3, 3, 3, 2, 100.0f, true);
			spawnWave(w);
		}
	}
	else if (Game::phase == Game::FIGHT_PHASE_2) {

		if (Game::enemyCount == 0) {
			Game::phase = Game::BOSS_PHASE;
			Wave w = Wave(1, 0, 0, 0, 5, 1000.0f, true);
			spawnWave(w);
			w = Wave(5, 5, 5, 5, 1, 1.0f, false);
			spawnWave(w);
		}
	}
	else if (Game::phase == Game::BOSS_PHASE) {
		if (Game::enemyCount == 0) {
			assets->PlaySound("boss_dead");
			Game::phase = Game::OVERWORLD_PHASE;
			//removeGate();
		}
	}
}

bool Game::inMathPhase() {
	return Game::phase == MATH_PHASE_0 ||
		Game::phase == MATH_PHASE_1 ||
		Game::phase == MATH_PHASE_2;
}

void Game::updateHUD() {
	energy.getComponent<TransformComponent>().width = Game::hero.energy;
}

void Game::updateEnergy() {
	Game::hero.energy = std::min(100.0f, Game::hero.energy + Game::hero.energyRefillRate);
}