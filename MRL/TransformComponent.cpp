#include "TransformComponent.h"


TransformComponent::TransformComponent() {
	this->position.Zero();
}

TransformComponent::TransformComponent(int scale) {
	this->position.x = 600;
	this->position.y = 400;
	this->scale = scale;
}

TransformComponent::TransformComponent(float x, float y) {
	this->position.x = x;
	this->position.y = y;
}

TransformComponent::TransformComponent(float x, float y, int w, int h, int scale) {
	this->position.x = x;
	this->position.y = y;
	this->height = h;
	this->width = w;
	this->scale = scale;
}

void TransformComponent::init() {
	this->velocity.Zero();
}

void TransformComponent::update() {
	// Should only take a step if it won't violate bounds
	position.x += velocity.x*speed;
	position.y += velocity.y*speed;

	if (isHero) {
		Game::hero.bowTransform->position = position;
	}
}
