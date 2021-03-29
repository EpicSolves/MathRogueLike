#pragma once
#include "Game.h"

class GameObject {
public:
	GameObject(const char *textureSheet, int x, int y);
	~GameObject();
	void update();
	void render();

private:
	int xpos;
	int ypos;
	SDL_Texture *objTexture;
	SDL_Rect sRect, dRect;
};

