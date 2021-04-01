#pragma once
#include "Components.h"
#include "Vector2D.h"
#include "Game.h"

struct TransformComponent : public Component {

	Vector2D position;
	Vector2D velocity;
	Vector2D center;

	bool isHero = false;

	int height = 32;
	int width = 32;
	int scale = 1;
	int speed = 3;

	TransformComponent();

	TransformComponent(int scale);

	TransformComponent(float x, float y);

	TransformComponent(float x, float y, int w, int h, int scale);

	void init() override;

	void update() override;
};
