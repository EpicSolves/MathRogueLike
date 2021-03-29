#pragma once

#include "SDL.h"
#include "AssetManager.h"

struct Animation {
public:
	int index;
	int frames;
	int speed;
	SDL_Texture *texture;

	Animation() {}
	Animation(int i, int f, int s, std::string tid);
	~Animation() {}
};
