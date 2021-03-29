#include "Animation.h"

Animation::Animation(int i, int f, int s, std::string tid) {
	this->index = i;
	this->frames = f;
	this->speed = s;
	this->texture = Game::assets->GetTexture(tid);
}