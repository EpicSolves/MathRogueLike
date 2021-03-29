#include "SpriteComponent.h"

void SpriteComponent::setTex(std::string texId) {
	texture = Game::assets->GetTexture(texId);
}

void SpriteComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
	sRect.x = sRect.y = 0;
	sRect.w = transform->width;
	sRect.h = transform->height;
}

void SpriteComponent::update() {

	// If we are animated, set source rect to current frame
	if (animated) {
		sRect.x = sRect.w * static_cast<int>((SDL_GetTicks() / speed) % this->frames);
		sRect.y = animIndex * transform->height;
	}
	
	dRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	dRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	dRect.w = transform->width*transform->scale;
	dRect.h = transform->height*transform->scale;
}

void SpriteComponent::draw() {
	TextureManager::Draw(texture, sRect, dRect, spriteFlip);
}

void SpriteComponent::AddAnimation(std::string aID, std::string tID, int i, int f, int s) {
	animations.emplace(aID, Animation(i, f, s, tID));
}

void SpriteComponent::Play(std::string aid) {
	frames = animations[aid].frames;
	animIndex = animations[aid].index;
	speed = animations[aid].speed;
	texture = animations[aid].texture;
}