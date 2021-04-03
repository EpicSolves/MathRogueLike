#pragma once
#include "SDL.h"
#include "Vector2D.h"

class ColliderComponent;

class Collision {
public:
	static bool ABB(const Vector2D &A, const SDL_Rect &B);
	static bool AABB(const SDL_Rect &rectA, const SDL_Rect &B);
	static bool AABB(const SDL_Rect &rectA, const SDL_Rect &B, uint8_t *side);
	static bool AABB(const ColliderComponent& A, const ColliderComponent& B);
};