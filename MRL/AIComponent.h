#pragma once
#include "ECS.h"
#include "Components.h"
#include "game.h"

#define STATE_NORMAL 0
#define STATE_FROZEN 1

class AIComponent : public Component {
public:
	AIComponent() {}
	AIComponent(bool cs) { canShoot = cs; }
	~AIComponent() {}
	int state = STATE_NORMAL;
	void init() override;
	void update() override;
	
private:
	ColliderComponent *collider;
	TransformComponent *transform;
	bool abilityIsActive = false;
	int abilityTimer = 5*60 + rand() % 10*60;

	int shootTimer = 0;
	float randA;
	float randB;
	bool canShoot = true;
};