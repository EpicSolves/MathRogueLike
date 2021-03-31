#include "Collision.h"
#include "ColliderComponent.h"
#include <cmath>

bool Collision::ABB(const Vector2D &A, const SDL_Rect &B) {
	if (A.x > B.x && A.x < B.x + B.w && A.y > B.y && A.y < B.y + B.h) {
		return true;
	}
	return false;
}

// Overloaded AABB which tells you which side of A that was hit
bool Collision::AABB(const SDL_Rect &A, const SDL_Rect &B, uint8_t *side) {
	if (A.x + A.w >= B.x &&
		B.x + B.w >= A.x &&
		A.y + A.h >= B.y &&
		B.y + B.h >= A.y) {
		if (A.x > B.x) {
			*side |= 0x01;
			printf("Hit left side\n");
		}
		if (A.x < B.x) {
			*side |= 0x02;
			printf("Hit right side\n");
		}
		if (A.y < B.y) {
			*side |= 0x04;
			printf("Hit bottom side\n");
		}
		if (B.y < A.y) {
			*side |= 0x08;
			printf("Hit top side\n");
		}
		return true;
	}
	return false;
}

bool Collision::AABB(const SDL_Rect &A, const SDL_Rect &B) {
	return A.x + A.w >= B.x &&
		B.x + B.w >= A.x &&
		A.y + A.h >= B.y &&
		B.y + B.h >= A.y;
}

bool Collision::AABB(const ColliderComponent& A, const ColliderComponent& B) {
	return AABB(A.collider, B.collider);
}