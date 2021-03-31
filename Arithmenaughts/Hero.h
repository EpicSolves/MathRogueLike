#pragma once
#include "ClassList.h"
#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include <map>
#include <vector>

class Hero {
public:
	/* A player will consist:
	   Transform component - for keeping track of position and movement velocity. 
	   Multiple sprite components (1 for each piece of gear.
	 */
	Hero();
	Entity& entity;
	TransformComponent *transform;
	SpriteComponent *sprite;
	KeyboardController *controller;
	ColliderComponent *collider;
	SpriteComponent *bowSprite;
	TransformComponent *bowTransform;
	//std::vector<TransformComponent *> gearPositions;

	void init();
	void addBow();
private:
};