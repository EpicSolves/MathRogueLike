#pragma once
#include "Components.h"
#include "Vector2D.h"

struct TransformComponent : public Component {

	Vector2D position;
	Vector2D velocity;
	Vector2D center;

	int height = 32;
	int width = 32;
	int scale = 1;
	int speed = 3;

	TransformComponent() {
		this->position.Zero();
	}

	TransformComponent(int scale) {
		this->position.x = 600;
		this->position.y = 400;
		this->scale = scale;
	}

	TransformComponent(float x, float y) {
		this->position.x = x;
		this->position.y = y;
	}

	TransformComponent(float x, float y, int w, int h, int scale) {
		this->position.x = x;
		this->position.y = y;
		this->height = h;
		this->width = w;
		this->scale = scale;
	}

	void init() override {
		this->velocity.Zero();
	}

	void update() override {
		// Should only take a step if it won't violate bounds
		position.x += velocity.x*speed;
		position.y += velocity.y*speed;
	}
};
