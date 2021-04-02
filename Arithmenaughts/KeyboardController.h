#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include "SDL_mixer.h"

class SpriteComponent;

class KeyboardController : public Component {
public:
	KeyboardController() {

	}
	TransformComponent *transform;
	SpriteComponent *sprite;
	void init() override;
	void update() override;
private:
	bool leftDebounce  = false;
	bool rightDebounce = false;
	bool upDebounce    = false;
	bool downDebounce  = false;
	int shotTimer = 0;
};