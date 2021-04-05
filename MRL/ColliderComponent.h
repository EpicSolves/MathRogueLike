#pragma once

#include <string>
#include "SDL.h"
#include "Components.h"
#include "TextureManager.h"

class TextureManager;

class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;
	bool isTile = false;

	int xOffset = 0;
	int yOffset = 0;

	SDL_Texture *tex;
	SDL_Rect sRect, dRect;

	TransformComponent *transform;

	ColliderComponent(std::string t);

	ColliderComponent(std::string t, int xOffset, int yOffset, int w, int h, bool isTile);

	ColliderComponent(std::string t, int xpos, int ypos, int size, bool isTile);

	void init() override;

	void update() override;

	void draw() override;
};