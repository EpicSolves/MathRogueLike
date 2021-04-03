#pragma once

#include "AssetManager.h"
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include <string>

class AssetManager;
class Animation;

class SpriteComponent : public Component {

private:
	TransformComponent *transform;
	SDL_Texture *texture;

	bool animated = false;
	int frames = 0;
	int speed = 100; // time between frame in ms
	std::map<std::string, Animation> animations;
	int ID = -1;
	Uint32 lastRender = 0;
	std::string currentAId = "";

public:
	bool isGear = false;
	bool isText = false;
	int animIndex = 0;
	int currentFrame = 0;
	SDL_Rect sRect, dRect;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	float angle = 0;

	SpriteComponent() = default;
	
	SpriteComponent(std::string texId) {
		setTex(texId);
	}

	SpriteComponent(bool isAnimated, int ID) {

		this->animated = isAnimated;
		this->ID = ID;

		//Animation idle = Animation(0, 8, 100);
		//Animation walk = Animation(1, 8, 100);

		//animations.emplace("idle", idle);
		//animations.emplace("walk", walk);

		//Play("idle");
		//setTex(texId);
	}

	~SpriteComponent() { }
	
	void setTex(std::string texId);

	void init() override;

	void update() override;

	void draw() override;

	void AddAnimation(std::string aID, std::string tID, int i, int f, int s);

	void Play(std::string aid);
};