#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "Components.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

class AssetManager {
public:
	AssetManager(Manager *man);
	~AssetManager();

	// Game objects
	void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string texId, std::size_t group);
	void SpawnEnemy(Vector2D pos, int scale, std::string texId);
	void PlaySound(std::string id);

	// Texture management
	void AddTexture(std::string id, const char *fp);
	SDL_Texture *GetTexture(std::string id);

	// Font management
	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font *GetFont(std::string id);

	// Sound management
	void AddSound(std::string id, const char *path);
	Mix_Chunk *GetSound(std::string id);

private:
	std::map<std::string, SDL_Texture *> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, Mix_Chunk *> sounds;
	Manager *manager;
};