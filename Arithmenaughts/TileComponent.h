#pragma once
#include "ECS.h"
#include "SDL.h"
#include "AssetManager.h"
#include "Vector2D.h"
#include "Game.h"
#include "TextureManager.h"

class AssetManager;

class TileComponent : public Component {
public:

	SDL_Texture *texture;
	SDL_Rect sRect, dRect;
	Vector2D position;

	TileComponent() = default;
	~TileComponent() {}

	TileComponent(int srcX, int srcY, int xPos, int yPos, int tsize, int tscale, std::string id);

	void draw() override;

	void update() override;
};