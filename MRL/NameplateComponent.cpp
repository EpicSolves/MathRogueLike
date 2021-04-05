#include "NameplateComponent.h"
NameplateComponent::NameplateComponent() {}
NameplateComponent::NameplateComponent(std::string texId) {
	texture = Game::assets->GetTexture(texId);
}
NameplateComponent::~NameplateComponent() {}

void NameplateComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
	stats = &entity->getComponent<StatsComponent>();
	sRect.x = sRect.y = 0;
	sRect.w = 32; // known for nameplates
	sRect.h = 32;
}

void NameplateComponent::update() {

	dRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
	dRect.y = static_cast<int>(transform->position.y - 15.0f) - Game::camera.y;
	dRect.w = 24.0f*transform->scale*(stats->health / 100.0f);
	dRect.h = 2.0f*transform->scale;
}

void NameplateComponent::setTex(std::string texId) {
	texture = Game::assets->GetTexture(texId);
}

void NameplateComponent::draw() {
	TextureManager::Draw(texture, sRect, dRect, NULL, SDL_FLIP_HORIZONTAL);
}