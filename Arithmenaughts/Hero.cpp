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

	// Add animations
	sprite->AddAnimation("player_idle", "player_idle_base", 0, 8, 100);
	sprite->AddAnimation("player_run", "player_run", 0, 8, 100);
	sprite->AddAnimation("player_dash", "player_dash", 0, 6, 55.555f);

	// Play an initial animation
	sprite->Play("player_idle");
}

void Hero::addBow() {
	auto& bow(Game::manager.addEntity());
	bow.addComponent<TransformComponent>(500, 500, 128, 128, 2);
	bow.addComponent<SpriteComponent>(true, 98);
	bow.getComponent<SpriteComponent>().AddAnimation("player_idle_bow_purple", "player_idle_bow_purple", 0, 8, 100);
	bow.getComponent<SpriteComponent>().AddAnimation("player_idle_bow_brown", "player_idle_bow_brown", 0, 8, 100);
	bow.getComponent<SpriteComponent>().Play("player_idle_bow_brown");
	bow.addGroup(Game::groupMisc);

	bowTransform = &bow.getComponent<TransformComponent>();
	bowSprite = &bow.getComponent<SpriteComponent>();
	bowSprite->isGear = true;
}