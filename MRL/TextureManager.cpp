#include "TextureManager.h"
#include "SDL.h"
#include "SDL_ttf.h"

TextureManager::TextureManager() {}
TextureManager::~TextureManager() {}

SDL_Texture *TextureManager::LoadTexture(const char *fileName) {

	SDL_Surface *tempSurface = IMG_Load(fileName);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, float angle, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, flip);
}

void TextureManager::DrawText(SDL_Texture *tex, SDL_Rect dest) {
	SDL_RenderCopyEx(Game::renderer, tex, NULL, &dest, NULL, NULL, SDL_FLIP_NONE);
}
