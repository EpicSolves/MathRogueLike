#include "AssetManager.h"

AssetManager::AssetManager() {
	manager = &Game::manager;

	// Could probably do this somewhere else, idk
	Mix_OpenAudio(44100, AUDIO_S16SYS, 6, 512);
}
AssetManager::~AssetManager() { }

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string texId, std::size_t group) {
	auto &projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(true, 1);
	projectile.getComponent<SpriteComponent>().AddAnimation("star_spin", "star_spin", 0, 2, 100);
	projectile.getComponent<SpriteComponent>().Play("star_spin");
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile", 4.0f, 1.0f, 22.0f, 27.0f, false);
	projectile.addGroup(group);
	// test
}

void AssetManager::SpawnEnemy(Vector2D pos, float scale, float health, bool canShoot, std::string texId) {
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 24, 32, scale);
	enemy.addComponent<SpriteComponent>(true, 0);
	enemy.getComponent<SpriteComponent>().AddAnimation(texId, texId, 0, 11, 100);
	enemy.getComponent<SpriteComponent>().Play("skeleton_idle");
	enemy.addComponent<ColliderComponent>("enemy", 0, 0, 24*scale, 32*scale, false);
	enemy.getComponent<ColliderComponent>().health = health;
	enemy.addComponent<AIComponent>(canShoot);
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