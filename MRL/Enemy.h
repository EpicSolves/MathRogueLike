#pragma once
#include "Vector2D.h"
#include "ECS.h"
#include "Components.h"
#include <string>

class Enemy {
public:
	Entity& entity;
	Enemy();
	//Enemy(const Enemy &old);
	//Enemy operator =(Enemy e);
	void init(Vector2D pos, float scale, float health, bool canShoot, std::string texId);
	~Enemy();

	// Basic components needed to control and see character
	TransformComponent *transform;
	SpriteComponent *sprite;
	ColliderComponent *collider;

	TransformComponent *nameplateTransform;

	// Stats
	float health = 100.0f;
private:
};

