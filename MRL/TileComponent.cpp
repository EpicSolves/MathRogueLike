#include "TileComponent.h"

TileComponent::TileComponent(int srcX, int srcY, int xPos, int yPos, int tsize, int tscale, std::string id) {

	texture = Game::assets->GetTexture(id);
	// texture = TextureManager::LoadTexture(fn);

	position.x = xPos;
	position.y = yPos;

	sRect.x = srcX;
	sRect.y = srcY;
	sRect.w = sRect.h = tsize;

	dRect.x = xPos;
	dRect.y = yPos;
	dRect.w = dRect.h = tsize * tscale;
}

void TileComponent::draw() {
	TextureManager::Draw(texture, sRect, dRect, 0.0f, SDL_FLIP_NONE);
}

void TileComponent::update() {
	dRect.x = position.x - Game::camera.x;
	dRect.y = position.y - Game::camera.y;
}