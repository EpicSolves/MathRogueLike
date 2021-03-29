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


#define RADTODEG(R)((180.0 * R) / PI)
#define PI 3.14159
#define SDL_SUCCESS 0
#define MAP_SCALE 4
#define TILE_SIZE 32

Map *map;
Manager manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

bool Game::isRunning = false;
bool Game::hasKey = false;
int Game::enemiesKilled = 0;
SDL_Rect Game::camera = { 0, 0, 1024, 768 };
AssetManager* Game::assets = new AssetManager(&manager);

bool Game::eligibleA = false;
bool Game::eligibleB = false;
int Game::numberAVal = 0;
int Game::numberBVal = 0;
int Game::counter = 0;

// Initialize phase to math phase and timer to 15 seconds
int Game::phase = Game::OVERWORLD_PHASE;
int Game::phaseTimer = MATH_PHASE_0_TIMER;
int Game::location = 0;

// Difficulty settings
int Game::enemySpawnTimer = 0;
int Game::spawnRate = 60 * 5;
std::map<std::string, int> Game::spelltimers = std::map<std::string, int>();

// Shoot speed based on initial math settings
float Game::shootSpeed = 5.0f;
const uint8_t *Game::keyState = 0;
 
int Map::mapWidth = TILE_SIZE * MAP_SCALE * 10;
int Map::mapHeight = TILE_SIZE * MAP_SCALE * 10;

// create a list of objects to be used in our game
auto& player(manager.addEntity());
auto& mana(manager.addEntity());
auto& health(manager.addEntity());
auto& numberA(manager.addEntity());
auto& numberB(manager.addEntity());
auto& healSpell(manager.addEntity());

// Create a list of label objects
auto& healthLabel(manager.addEntity());
auto& manaLabel(manager.addEntity());
auto& score(manager.addEntity());

// Create out groups
auto& players(manager.getGroup(Game::groupPlayers));
auto& miscs(manager.getGroup(Game::groupMisc));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& headsUpDisplay(manager.getGroup(Game::groupHUD));
auto& enemyProjectiles(manager.getGroup(Game::groupEnemyProjectiles));
auto& playerProjectiles(manager.getGroup(Game::groupPlayerProjectiles));
auto& collectibles(manager.getGroup(Game::groupCollectibles));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& UILabels(manager.getGroup(Game::groupUILabels));
auto& mapTiles(manager.getGroup(Game::groupMap));
auto& mapColliders(manager.getGroup(Game::groupMapColliders));
auto& extraMapTiles(manager.getGroup(Game::groupExtraMapTiles));

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

	printf("Window created successfully\n");

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
	assets->AddTexture("NewtonsGroveOverworld_Tileset", "Assets/NewtonsGroveOverworld_Tileset.png");
	assets->AddTexture("NewtonsGroveD0_Tileset", "Assets/NewtonsGroveD0_Tileset.png");
	assets->AddTexture("player_idle", "Assets/player_idle.png");
	assets->AddTexture("player_run", "Assets/player_run.png");
	assets->AddTexture("numbers", "Assets/numbers.png");
	assets->AddTexture("star_spin", "Assets/star_spin.png");
	assets->AddTexture("mana", "Assets/mana.png");
	assets->AddTexture("health", "Assets/health.png");
	assets->AddTexture("spell_heal", "Assets/spell_heal.png");
	assets->AddTexture("skeleton_idle", "Assets/skeleton_idle.png");

	// Load fonts
	assets->AddFont("verdana", "Assets/Verdana.ttf", 32);

	// Load sounds
	assets->AddSound("star_shot", "Assets/star.wav");
	assets->AddSound("enemy_hit", "Assets/enemy_hit.wav");
	assets->AddSound("heal_cast", "Assets/heal.wav");
	assets->AddSound("player_hit", "Assets/player_hit.wav");
	assets->AddSound("game_over", "Assets/game_over.wav");
	assets->AddSound("mana_up", "Assets/mana_up.wav");
	assets->AddSound("skeleton_dead", "Assets/skeleton_dead.wav");
	
	// Load the map
	map = new Map("NewtonsGroveOverworld_Tileset", MAP_SCALE, TILE_SIZE);
	map->LoadMap("Assets/NewtonsGroveOverworld.map", 10, 10, "NewtonsGroveOverworld_Tileset");

	// Get an initial keyboard state
	Game::keyState = SDL_GetKeyboardState(NULL);

	// Create Labels
	SDL_Color white = { 255, 255, 255, 200 };
	healthLabel.addComponent<UILabel>(10, 10, "Health", "verdana", white, "health_label");
	healthLabel.addGroup(groupUILabels);
	manaLabel.addComponent<UILabel>(10, 50, "Mana", "verdana", white, "health_label");
	manaLabel.addGroup(groupUILabels);
	score.addComponent<UILabel>(10, 90, "Score: 0", "verdana", white, "score_label");
	score.addGroup(groupUILabels);

	// Add components to our player
	player.addComponent<TransformComponent>(500, 500, 128, 128, 2);
	player.addComponent<SpriteComponent>(true, 99);
	player.getComponent<SpriteComponent>().AddAnimation("player_idle", "player_idle", 0, 8, 100);
	player.getComponent<SpriteComponent>().AddAnimation("player_run", "player_run", 0, 8, 100);
	player.getComponent<SpriteComponent>().Play("player_idle");
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player", 49*2, 38*2, 27.0*2, 37.0*2, false);
	player.addGroup(groupPlayers);

	// Add health bar display
	health.addComponent<TransformComponent>(10.0f, 10.0f, 100.0f, 10.0f, 4);
	health.addComponent<SpriteComponent>("health");
	health.addComponent<StatsComponent>("health", 100.0f);
	health.addGroup(groupHUD);

	// Add mana bar display
	mana.addComponent<TransformComponent>(10.0f, 55.0f, 100.0f, 10.0f, 4);
	mana.addComponent<SpriteComponent>("mana");
	mana.addComponent<StatsComponent>("mana", 100.0f);
	mana.addGroup(groupHUD);

	// The first digit of the number pop-up interface
	numberA.addComponent<TransformComponent>(300, 800, 32, 32, 4);
	numberA.addComponent<SpriteComponent>("numbers");
	numberA.getComponent<SpriteComponent>().sRect.x = 0;
	numberA.getComponent<SpriteComponent>().sRect.y = 0;
	numberA.getComponent<SpriteComponent>().sRect.w = 16;
	numberA.addGroup(groupHUD);

	// The second digit of the number pop-up interface
	numberB.addComponent<TransformComponent>(400, 800, 32, 32, 4);
	numberB.addComponent<SpriteComponent>("numbers");
	numberB.getComponent<SpriteComponent>().sRect.x = 0;
	numberB.getComponent<SpriteComponent>().sRect.y = 0;
	numberB.getComponent<SpriteComponent>().sRect.w = 16;
	numberB.addGroup(groupHUD);

	// Add a spell to the bottom of the screen
	healSpell.addComponent<TransformComponent>(2);
	healSpell.addComponent<SpriteComponent>("spell_heal");
	healSpell.addComponent<ColliderComponent>("spell");
	Game::spelltimers["heal"] = 0;
	healSpell.addGroup(groupHUD);


}

/**************************************************************************/
/*                                                                        */
/* Handle events                                                          */
/*                                                                        */
/* Poll for events and then handle. This happens before we update stuff.  */
/*                                                                        */
/**************************************************************************/
void Game::handleEvents() {
	
	SDL_PollEvent(&Game::event);
	switch (Game::event.type) {
		case SDL_QUIT: {
			this->isRunning = false;
			break;
		}
		default: {
			break;
		}
	}

	// If we are in math phase
	if (Game::phase == Game::MATH_PHASE_0) {

		// Show the num pad
		showSolvePad();

		// Check if the user solved a problem
		if (solvedProblem()) {

			// Indicate we solved it
			Game::assets->PlaySound("mana_up");

			// Increment shot speed
			Game::shootSpeed += 0.25f;

			// Generate a new problem
			generatenewProblem();
		}
	} else {  // We are in a fighting mdoe

		int mouseX, mouseY;
		float mVal = mana.getComponent<StatsComponent>().getResource("mana");

		// Check if the user is in pad solve mode
		if (inSolvePadMode()) {

			// Show the solve pad
			showSolvePad();

			// Determine if the user answered a question to get back mana
			if (solvedProblem()) {

				// Indicate the problem was solved correctly
				Game::assets->PlaySound("mana_up");

				// Increment mana
				mana.getComponent<StatsComponent>().setResource("mana", std::min((int)mana.getComponent<StatsComponent>().getResource("mana") + 15, 100));

				// Generate a new problem
				generatenewProblem();
			}
		} else {  // Normal fighting mode (no solve pad open)

			// Hide the solve pad 
			hideSolvePad();

			// Handle any spells the user may have clicked
			if (Game::keyState[SDL_SCANCODE_KP_1]) {
				if (Game::spelltimers["heal"] == 0) {
					if (mVal > 10.0) {
						Game::assets->PlaySound("heal_cast");
						float hVal = health.getComponent<StatsComponent>().getResource("health");
						hVal += 35;
						health.getComponent<StatsComponent>().setResource("health", std::min(100.0f, hVal));
						mana.getComponent<StatsComponent>().setResource("mana", std::max(0.0f, mVal - 10.0f));
						Game::spelltimers["heal"] = 180;
					}
				}
			}
		}
	}
}

/**************************************************************************/
/*                                                                        */
/* Update objects based on what happened with the events.                 */
/*                                                                        */
/**************************************************************************/
void Game::update() {
	if (Game::location == 0) {
		// Check if the user entered a dungeon

		if (player.getComponent<ColliderComponent>().collider.x > Map::mapWidth) {
			Game::phase = Game::MATH_PHASE_0;
			Game::phaseTimer = MATH_PHASE_0_TIMER;
			Game::location = 1;
			printf("Hello\n");

			// Clean it up
			free(map);
			cleanMapTiles();

			// Load dungeon 0
			Map::mapWidth = TILE_SIZE * MAP_SCALE * 10;
			Map::mapHeight = TILE_SIZE * MAP_SCALE * 10;
			map = new Map("NewtonsGroveD0_Tileset", MAP_SCALE, TILE_SIZE);
			map->LoadMap("Assets/NewtonsGroveD0.map", 10, 10, "NewtonsGroveD0_Tileset");
			player.getComponent<TransformComponent>().position.x = 64;
			player.getComponent<ColliderComponent>().update();
			
			// Shut the gate
			auto& newTile(manager.addEntity());
			newTile.addComponent<TransformComponent>(0, 6 * TILE_SIZE* MAP_SCALE, TILE_SIZE, TILE_SIZE, MAP_SCALE);
			newTile.addComponent<SpriteComponent>("NewtonsGroveD0_Tileset");
			
			newTile.getComponent<SpriteComponent>().sRect.x = 9 * 32;
			newTile.getComponent<SpriteComponent>().sRect.y = 32;
			newTile.addGroup(groupExtraMapTiles);

			auto& gateACollider(manager.addEntity());
			gateACollider.addComponent<ColliderComponent>("GateA", 0, 6 * TILE_SIZE*MAP_SCALE, 32 * MAP_SCALE, true);
			gateACollider.addGroup(groupMapColliders);

			// Shut the gate
			auto& newTile2(manager.addEntity());
			newTile2.addComponent<TransformComponent>(0, 7 * TILE_SIZE* MAP_SCALE, TILE_SIZE, TILE_SIZE, MAP_SCALE);
			newTile2.addComponent<SpriteComponent>("NewtonsGroveD0_Tileset");
			newTile2.getComponent<SpriteComponent>().sRect.x = 0 * 32;
			newTile2.getComponent<SpriteComponent>().sRect.y = 2* 32;
			// newTile2.addComponent<ColliderComponent>("GateB", 0, 7 * TILE_SIZE* MAP_SCALE, 32 * MAP_SCALE, false);
			newTile2.addGroup(groupExtraMapTiles);

			auto& gateBCollider(manager.addEntity());
			gateBCollider.addComponent<ColliderComponent>("GateB", 0, 7 * TILE_SIZE*MAP_SCALE, 32 * MAP_SCALE, true);
			gateBCollider.addGroup(groupMapColliders);
		}
	}
	else if (Game::location == 1) {
		if (player.getComponent<ColliderComponent>().collider.x < -50) {
			// Clean it up
			free(map);
			cleanMapTiles();

			// Load dungeon 0
			Map::mapWidth = TILE_SIZE * MAP_SCALE * 10;
			Map::mapHeight = TILE_SIZE * MAP_SCALE * 10;
			map = new Map("NewtonsGroveOverworld_Tileset", MAP_SCALE, TILE_SIZE);
			map->LoadMap("Assets/NewtonsGroveOverworld.map", 10, 10, "NewtonsGroveOverworld_Tileset");
			player.getComponent<TransformComponent>().position.x = 1000;
			player.getComponent<ColliderComponent>().update();
			Game::location = 0;
		}
	}
	if (Game::phase == Game::MATH_PHASE_0) {
		Game::phaseTimer--;
		if (Game::phaseTimer <= 0) {
			Game::phase = Game::FIGHT_PHASE_0;
			Game::phaseTimer = FIGHT_PHASE_0_TIMER;
		}
	}
	else if (Game::phase == Game::FIGHT_PHASE_0) {
		Game::phaseTimer--;
		if (Game::phaseTimer <= 0) {
			removeGate();
			Game::phase = Game::OVERWORLD_PHASE;
		}
		Game::counter++;
		if (Game::counter >= 180) {
			int enemyX, enemyY, playerX, playerY;
			for (auto& e : enemies) {

				enemyX = e->getComponent<TransformComponent>().position.x + 15.0f;
				enemyY = e->getComponent<TransformComponent>().position.y + 15.0f;

				playerX = player.getComponent<TransformComponent>().position.x + 58 * player.getComponent<TransformComponent>().scale;
				playerY = player.getComponent<TransformComponent>().position.y + 52 * player.getComponent<TransformComponent>().scale;

				// Calculate x and y velocity
				float xDist = playerX - enemyX;
				float yDist = enemyY - playerY;
				float angle = std::atan2(yDist, xDist);
				float xVel = std::cos(angle);
				float yVel = std::sin(angle);

				// Create a little explosion to start the game	
				assets->CreateProjectile(Vector2D(enemyX, enemyY), Vector2D(xVel, -yVel), 600, 4, "star", groupEnemyProjectiles);

			}
			Game::counter = 0;
		}
		Game::enemySpawnTimer = std::max(0, Game::enemySpawnTimer - 1);
		if (Game::enemySpawnTimer == 0) {
			Game::assets->SpawnEnemy(Vector2D(rand() % 10 * 32 * MAP_SCALE, rand() % 10 * 32 * MAP_SCALE), 2, "player");
			Game::enemySpawnTimer = Game::spawnRate -= 2;
		}
	}

	Game::spelltimers["heal"] = std::max(0, Game::spelltimers["heal"] - 1);

	// Keep record of players current position to handle collision after the update
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();


	// Ensure if player hits wall, he goes back to where he was
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	for (auto& c : mapColliders) {
		
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

		// If there was a collision, see which bound collider
		// Check if top of player hit bottom of box
		if (Collision::ABB(Vector2D(playerCol.x + 10, playerCol.y), cCol)) {
			player.getComponent<TransformComponent>().position.y = playerPos.y + 0.01f;
			player.getComponent<ColliderComponent>().update();
			playerCol = player.getComponent<ColliderComponent>().collider;
		}

		// Check if left of player hit right of box
		if (Collision::ABB(Vector2D(playerCol.x, playerCol.y+10), cCol)) {
			player.getComponent<TransformComponent>().position.x = playerPos.x + 0.01f;
			player.getComponent<ColliderComponent>().update();
			playerCol = player.getComponent<ColliderComponent>().collider;
		}

		// Check if right of player hit left of box
		if (Collision::ABB(Vector2D(playerCol.x + playerCol.w, playerCol.y + 10), cCol)) {
			player.getComponent<TransformComponent>().position.x = playerPos.x - 0.01f;
			player.getComponent<ColliderComponent>().update();
			playerCol = player.getComponent<ColliderComponent>().collider;
		}

		// Check if bottom of player hit top of box
		if (Collision::ABB(Vector2D(playerCol.x + 10, playerCol.y + playerCol.h), cCol)) {
			player.getComponent<TransformComponent>().position.y = playerPos.y - 0.01f;
			player.getComponent<ColliderComponent>().update();
			playerCol = player.getComponent<ColliderComponent>().collider;
		}
	}
	
	// Check if player hit enemy
	for (auto& p : playerProjectiles) {
		for (auto& e : enemies) {
			if (Collision::AABB(e->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
				p->destroy();
				e->getComponent<ColliderComponent>().health -= 30.0f;

				if (e->getComponent<ColliderComponent>().health <= 0.0f) {

					// Enemy was killed
					e->destroy();
			
					// increment score
					Game::enemiesKilled++;
					for (auto& l : UILabels) {
						if (l->getComponent<UILabel>().tagId == "score_label")
							l->destroy();
					}

					auto& newScore(manager.addEntity());
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

	// If enemy hit player
	for (auto& p : enemyProjectiles) {
		
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
			p->destroy();
			Game::assets->PlaySound("player_hit");
			float hVal = health.getComponent<StatsComponent>().getResource("health");
			health.getComponent<StatsComponent>().setResource("health", hVal - 20.0f);
			if (health.getComponent<StatsComponent>().getResource("health") <= 0) {
				Game::assets->PlaySound("game_over");
				player.getComponent<TransformComponent>().position.x = 800*MAP_SCALE;
				player.getComponent<TransformComponent>().position.y = 600*MAP_SCALE;
				health.getComponent<StatsComponent>().setResource("health", 100.0f);
				SDL_Delay(5000);
				Game::isRunning = false;
			}
		}
	}
	
	camera.x = std::max(0.0f, playerPos.x - 400);
	camera.y = std::max(0.0f, playerPos.y - 320);
	camera.x = std::min(camera.x, Map::mapWidth - camera.w);
	camera.y = std::min(camera.y, Map::mapHeight - camera.h);
	
	// Keep health bar in top left of view
	health.getComponent<TransformComponent>().position.x = camera.x + 10.0;
	health.getComponent<TransformComponent>().position.y = camera.y + 10.0;

	// Keep mana bar in top left of view
	mana.getComponent<TransformComponent>().position.x = camera.x + 10.0;
	mana.getComponent<TransformComponent>().position.y = camera.y + 50.0;

	// Keep spells on bottom
	healSpell.getComponent<TransformComponent>().position.x = camera.x + camera.w / 2 - healSpell.getComponent<TransformComponent>().width*healSpell.getComponent<TransformComponent>().scale / 2;
	healSpell.getComponent<TransformComponent>().position.y = camera.y + camera.h - healSpell.getComponent<TransformComponent>().height*healSpell.getComponent<TransformComponent>().scale - 10.0f;
	
	// Check spell CDs
	if (Game::spelltimers["heal"] == 0) {
		healSpell.getComponent<SpriteComponent>().sRect.x = 0;
	} else {
		healSpell.getComponent<SpriteComponent>().sRect.x = 32;
	}
}

/**************************************************************************/
/*                                                                        */
/* Draw the new stuff on the screen.                                      */
/*                                                                        */
/**************************************************************************/
void Game::render() {

	SDL_RenderClear(renderer);
	for (auto& t : mapTiles) t->draw();
	for (auto& t : extraMapTiles) t->draw();
	for (auto& c : colliders) c->draw();
	for (auto& p : players) p->draw();
	for (auto& m : miscs) m->draw();
	for (auto& e : enemies) e->draw();
	for (auto& c : collectibles) c->draw();
	for (auto& p : playerProjectiles) p->draw();
	for (auto& p : enemyProjectiles) p->draw();
	for (auto& h : headsUpDisplay) h->draw();
	for (auto& l : UILabels) l->draw();
	SDL_RenderPresent(renderer);
}

// Show / hide number pad, check if user solved proble, generate a new problem
void Game::showSolvePad() {
	numberA.getComponent<SpriteComponent>().sRect.x = 16 * Game::numberAVal;
	numberB.getComponent<SpriteComponent>().sRect.x = 16 * Game::numberBVal;
	numberA.getComponent<TransformComponent>().position.x = camera.x + 200;
	numberB.getComponent<TransformComponent>().position.x = camera.x + 300;
	numberA.getComponent<TransformComponent>().position.y = camera.y + camera.h - 200;
	numberB.getComponent<TransformComponent>().position.y = camera.y + camera.h - 200;
}
void Game::hideSolvePad() {
	numberA.getComponent<TransformComponent>().position.x = -200;
	numberB.getComponent<TransformComponent>().position.x = -200;
}
bool Game::solvedProblem() {
	return Game::keyState[SDL_SCANCODE_KP_1 + Game::numberAVal + Game::numberBVal - 1] ||
		   Game::keyState[SDL_SCANCODE_KP_0 + Game::numberAVal + Game::numberBVal];
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