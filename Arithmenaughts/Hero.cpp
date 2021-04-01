#include "Hero.h"

Hero::Hero() : entity(Game::manager.addEntity()) {

}
void Hero::init() {
	entity.addComponent<TransformComponent>(500, 500, 128, 128, 2);
	entity.addComponent<SpriteComponent>(true, 99);
	entity.addComponent<KeyboardController>();
	entity.addComponent<ColliderComponent>("Player", 49 * 2, 38 * 2, 27.0 * 2, 37.0 * 2, false);
	entity.addGroup(Game::groupPlayers);

	transform = &entity.getComponent<TransformComponent>();
	sprite = &entity.getComponent<SpriteComponent>();
	controller = &entity.getComponent<KeyboardController>();
	collider = &entity.getComponent<ColliderComponent>();

	// Set hero
	transform->isHero = true;

	// Add animations
	sprite->AddAnimation("player_idle", "player_idle_base", 0, 8, 100);
	sprite->AddAnimation("player_run", "player_run_base", 0, 8, 100);
	sprite->AddAnimation("player_dash", "player_dash", 0, 6, 55.555f);

	// Play an initial animation
	sprite->Play("player_idle");
}

void Hero::setBow(std::string color) {
	
	auto& bow(Game::manager.addEntity());
	bow.addComponent<TransformComponent>(500, 500, 128, 128, 2);
	bow.addComponent<SpriteComponent>(true, 98);
	if (color == "brown") {
		bow.getComponent<SpriteComponent>().AddAnimation("player_idle", "player_idle_bow_brown", 0, 8, 100);
		bow.getComponent<SpriteComponent>().AddAnimation("player_run", "player_run_bow_brown", 0, 8, 100);
	}
	else {
		bow.getComponent<SpriteComponent>().AddAnimation("player_idle", "player_idle_bow_purple", 0, 8, 100);
		bow.getComponent<SpriteComponent>().AddAnimation("player_run", "player_run_bow_purple", 0, 8, 100);
	}
	bow.addGroup(Game::groupGear);
	bow.getComponent<SpriteComponent>().Play("player_idle");

	bowTransform = &bow.getComponent<TransformComponent>();
	bowSprite = &bow.getComponent<SpriteComponent>();
	bowSprite->isGear = true;
}

void Hero::calculateStats() {
	
	// Update baseline stats to reflect gear
	strength = 5.0 + bowStrength + solveStrengthBonus;
	agility = 5.0 + bowAgility + solveAgilityBonus;
	critical = 5.0f + bowCritical + solveCriticalBonus;
	stamina = 5.0f + bowStamina + solveStaminaBonus;
	
	// Resulting stats based on non-dependent stats
	hitPoints = 100.0f*(1.0f + stamina / 500.0f);
	shotSpeed = agility;
	damageIncreaseCoeff = 1.0f + (agility + strength) / 100.0f;
	damageReductionCoeff = 1.0f - stamina / 500.0f;
	damagerPerShot = (strength*3.0 + agility)*damageIncreaseCoeff;

	// Clear the stats changed flag
	statsChanged = false;
}

bool Hero::didCrit() {
	return 1 + (rand() % 100) <= critical;
}