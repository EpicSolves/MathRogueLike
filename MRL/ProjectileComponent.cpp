#include "ProjectileComponent.h"

ProjectileComponent::ProjectileComponent(int rng, int sp, Vector2D vel) {
	range = rng;
	speed = sp;
	velocity = vel;
}
ProjectileComponent::~ProjectileComponent() {}

void ProjectileComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
	transform->velocity = velocity;
	transform->speed = speed;
}

void ProjectileComponent::update() {
	distance += speed;
	if (distance > range) {
		entity->destroy();
	}
	else if (transform->position.x > Game::camera.x + Game::camera.w ||
		transform->position.x < Game::camera.x ||
		transform->position.y < Game::camera.y ||
		transform->position.y > Game::camera.y + Game::camera.h) {
		entity->destroy();
	}
}