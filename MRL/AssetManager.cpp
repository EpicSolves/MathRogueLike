#include "AssetManager.h"
#include "Enemy.h"

class Enemy;

AssetManager::AssetManager() {
	manager = &Game::manager;

	// Could probably do this somewhere else, idk
	Mix_OpenAudio(44100, AUDIO_S16SYS, 6, 512);
}
AssetManager::~AssetManager() { }

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string texId, std::size_t group, float angle) {
	auto &projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
	projectile.addComponent<SpriteComponent>(true, 1);
	projectile.getComponent<SpriteComponent>().AddAnimation(texId, texId, 0, 2, 100);
	projectile.getComponent<SpriteComponent>().Play(texId);
	projectile.getComponent<SpriteComponent>().angle = -1.0*angle * 180 / 3.1415;
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile", 4.0f, 1.0f, 22.0f, 27.0f, false);
	projectile.getComponent<ColliderComponent>().collider.x = pos.x;
	projectile.getComponent<ColliderComponent>().collider.y = pos.y;
	projectile.addGroup(group);
	// test
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string texId, std::size_t group) {
	auto &projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(true, 1);
	projectile.getComponent<SpriteComponent>().AddAnimation(texId, texId, 0, 2, 100);
	projectile.getComponent<SpriteComponent>().Play(texId);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile", 4.0f, 1.0f, 22.0f, 27.0f, false);
	projectile.getComponent<ColliderComponent>().collider.x = pos.x;
	projectile.getComponent<ColliderComponent>().collider.y = pos.y;
	projectile.addGroup(group);
	// test
}

void AssetManager::SpawnEnemy(Vector2D pos, float scale, float health, bool canShoot, std::string texId) {
	printf("In spawn enemy\n");

	// Create new entity
	auto& enemy(manager->addEntity());

	// Add a position component
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 24, 32, scale);

	// Add the main sprite of the enemy
	enemy.addComponent<SpriteComponent>(true, 0);
	enemy.getComponent<SpriteComponent>().AddAnimation(texId, texId, 0, 11, 100);
	enemy.getComponent<SpriteComponent>().Play("skeleton_idle");

	// Make the enemy be collidable
	enemy.addComponent<ColliderComponent>("enemy", 0, 0, 24 * scale, 32 * scale, false);
	enemy.getComponent<ColliderComponent>().collider.x = pos.x;
	enemy.getComponent<ColliderComponent>().collider.y = pos.y;

	// Give the enemy stats
	enemy.addComponent<StatsComponent>();

	// Give the enemy brains
	enemy.addComponent<AIComponent>(canShoot);

	// Give the enemy a nameplate
	enemy.addComponent<NameplateComponent>("enemy_nameplate");

	// Add the enemy to the group of enemy entities
	enemy.addGroup(Game::groupEnemies);
	Game::enemyCount++;
}

void AssetManager::PlaySound(std::string id) {
	Mix_PlayChannel(-1, sounds[id], 0);
}

void AssetManager::AddTexture(std::string id, const char *fp) {
	textures.emplace(id, TextureManager::LoadTexture(fp));
}

SDL_Texture *AssetManager::GetTexture(std::string id) {
	return textures[id];
}
void AssetManager::AddFont(std::string id, std::string path, int fontSize) {
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}
TTF_Font* AssetManager::GetFont(std::string id) {
	return fonts[id];
}

void AssetManager::AddSound(std::string id, const char *path) {
	sounds.emplace(id, Mix_LoadWAV(path));
}

Mix_Chunk *AssetManager::GetSound(std::string id) {
	return sounds[id];
}