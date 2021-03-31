#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component {
public:
	ProjectileComponent(int rng, int sp, Vector2D vel);
	~ProjectileComponent();

	void init() override;

	void update() override;

private:

	Vector2D velocity;
	TransformComponent *transform;
	int range = 0;
	int speed = 0;
	int distance = 0;
};