#include "Enemy.h"
#include "ECS.h"
#include "Components.h"
#include "Game.h"

Enemy::Enemy() : entity(Game::manager.addEntity()) {
	printf("created\n");
}

//Enemy::Enemy(const Enemy &old) : entity(Game::manager.addEntity()) { }

void Enemy::init(Vector2D pos, float scale, float health, bool canShoot, std::string texId) {
	printf("Inited\n");
	entity.addComponent<TransformComponent>(pos.x, pos.y, 24, 32, scale);
	entity.addComponent<SpriteComponent>(true, 0);
	entity.getComponent<SpriteComponent>().AddAnimation(texId, texId, 0, 11, 100);
	entity.getComponent<SpriteComponent>().Play("skeleton_idle");
	entity.addComponent<ColliderComponent>("enemy", 0, 0, 24 * scale, 32 * scale, false);
	entity.getComponent<ColliderComponent>().health = health;
	entity.getComponent<ColliderComponent>().collider.x = pos.x;
	entity.getComponent<ColliderComponent>().collider.y = pos.y;
	entity.addComponent<AIComponent>(canShoot);
	entity.addGroup(Game::groupEnemies);

	collider = &entity.getComponent<ColliderComponent>();
	transform = &entity.getComponent<TransformComponent>();

	auto& np(Game::manager.addEntity());
	np.addComponent<TransformComponent>(pos.x, pos.y - 15.0f, 24, 2, scale);
	np.addComponent<SpriteComponent>("enemy_nameplate");
	np.addGroup(Game::groupNameplates);
	nameplateTransform = &np.getComponent<TransformComponent>();
	
	Game::enemyCount++;
}
Enemy::~Enemy() { 
	printf("destroyed\n");
}

/*
Enemy Enemy::operator =(Enemy e) {
	Enemy temp = e;
	return temp;
}*/