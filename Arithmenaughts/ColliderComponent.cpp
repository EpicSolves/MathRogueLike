#pragma once

#include "ColliderComponent.h"
#include <string>

class TextureManager;


ColliderComponent::ColliderComponent(std::string t) {
	tag = t;
}

ColliderComponent::ColliderComponent(std::string t, int xOffset, int yOffset, int w, int h, bool isTile) {
	this->collider.w = w;
	this->collider.h = h;
	this->xOffset = xOffset;
	this->yOffset = yOffset;
	tag = t;
	this->isTile = isTile;
}

ColliderComponent::ColliderComponent(std::string t, int xpos, int ypos, int size, bool isTile) {
	tag = t;
	collider.x = xpos;
	collider.y = ypos;
	collider.h = collider.w = size;
	this->isTile = isTile;
}

void ColliderComponent::init() {
	if (!entity->hasComponent<TransformComponent>())
		entity->addComponent<TransformComponent>();
	transform = &entity->getComponent<TransformComponent>();

	tex = TextureManager::LoadTexture("Assets/Textures/TolTex.png");
	sRect = { 0, 0, 32, 32 };
	dRect = { collider.x, collider.y, collider.w, collider.h };
}

void ColliderComponent::update()  {
	if (!isTile) {
		collider.x = static_cast<int>(transform->position.x) + xOffset;
		collider.y = static_cast<int>(transform->position.y) + yOffset;

		// Don't change w/h unless we really want to, so we can make smaller hitboxes
		// collider.w = transform->width * transform->scale;
		// collider.h = transform->height * transform->scale;
	}

	dRect.x = collider.x - Game::camera.x;
	dRect.y = collider.y - Game::camera.y;
}

void ColliderComponent::draw() {
	if (DRAW_COLLISION_BOXES)
		TextureManager::Draw(tex, sRect, dRect, SDL_FLIP_NONE);
}
