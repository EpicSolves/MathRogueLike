#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char *textureSheet, int x, int y) {
	this->objTexture = TextureManager::LoadTexture(textureSheet);
	this->xpos = x;
	this->ypos = y;
}

void GameObject::update() {

	// Continuous-response keys
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_RIGHT])
		this->xpos += 2;
	if (keystate[SDL_SCANCODE_LEFT])
		this->xpos -= 2;
	if (keystate[SDL_SCANCODE_UP])
		this->ypos -= 2;
	if (keystate[SDL_SCANCODE_DOWN])
		this->ypos += 2;

	this->sRect.h = 32;
	this->sRect.w = 32;
	this->sRect.x = 0;
	this->sRect.y = 0;
	this->dRect.x = this->xpos;
	this->dRect.y = this->ypos;
	this->dRect.w = this->sRect.w * 2;
	this->dRect.h = this->sRect.h * 2;
}

void GameObject::render() {
	SDL_RenderCopy(Game::renderer, this->objTexture, &this->sRect, &this->dRect);
}