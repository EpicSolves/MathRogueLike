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
		if (!isGear) {
			Uint32	currentRender = SDL_GetTicks();
			if ((currentRender - lastRender) >= speed) {
				currentFrame = (currentFrame + 1) % this->frames;
				lastRender = currentRender;
			}
		}
		sRect.x = sRect.w * currentFrame;
		sRect.y = animIndex * transform->height;
	}
	
	dRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	dRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	dRect.w = transform->width*transform->scale;
	dRect.h = transform->height*transform->scale;
}

void SpriteComponent::draw() {
	TextureManager::Draw(texture, sRect, dRect, angle, spriteFlip);
}

void SpriteComponent::AddAnimation(std::string aID, std::string tID, int i, int f, int s) {
	animations.emplace(aID, Animation(i, f, s, tID));
}

void SpriteComponent::Play(std::string aid) {
	if (aid != currentAId) {
		frames = animations[aid].frames;
		animIndex = animations[aid].index;
		speed = animations[aid].speed;
		texture = animations[aid].texture;
		currentFrame = 0;
		lastRender = 0;
		currentAId = aid;
	}
}