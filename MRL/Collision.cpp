#include "Collision.h"
#include "ColliderComponent.h"
#include <cmath>

bool Collision::ABB(const Vector2D &A, const SDL_Rect &B) {
	if (A.x >= B.x && A.x <= B.x + B.w && A.y >= B.y && A.y <= B.y + B.h) {
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
	
	if (A.x + A.w >= B.x &&
		B.x + B.w >= A.x &&
		A.y + A.h >= B.y &&
		B.y + B.h >= A.y) {
		printf("%d %d %d %d\n", A.x, A.y, B.x, B.y);
		return true;
	}
	return false;
}

bool Collision::AABB(const ColliderComponent& A, const ColliderComponent& B) {
	return AABB(A.collider, B.collider);
}

void Collision::PlayerCollision(const ColliderComponent& A, bool cp[8]) {
	SDL_Rect cCol = A.collider;

	// Check top of player hit bottom of box
	if (Collision::ABB(Vector2D(Game::hero.collider->collider.x + Game::hero.collider->collider.w/2, Game::hero.collider->collider.y), cCol))
		cp[1] = true;

	// Check left of player hit right of box
	if (Collision::ABB(Vector2D(Game::hero.collider->collider.x, Game::hero.collider->collider.y + Game::hero.collider->collider.h/2), cCol))
		cp[3] = true;

	// Check right of player hit left of box
	if (Collision::ABB(Vector2D(Game::hero.collider->collider.x + Game::hero.collider->collider.w, Game::hero.collider->collider.y + Game::hero.collider->collider.h/2), cCol))
		cp[4] = true;
		
	// Check if bottom of player hit top of box
	if (Collision::ABB(Vector2D(Game::hero.collider->collider.x + Game::hero.collider->collider.w/2, Game::hero.collider->collider.y + Game::hero.collider->collider.h), cCol))
		cp[6] = true;

	// Check top right corner
	if (Collision::ABB(Vector2D(Game::hero.collider->collider.x + Game::hero.collider->collider.w, Game::hero.collider->collider.y), cCol))
		cp[2] = true;

	// Check top left corner
	if (Collision::ABB(Vector2D(Game::hero.collider->collider.x, Game::hero.collider->collider.y), cCol))
		cp[0] = true;

	// Check bottom left corner
	if (Collision::ABB(Vector2D(Game::hero.collider->collider.x, Game::hero.collider->collider.y + Game::hero.collider->collider.h), cCol))
		cp[5] = true;

	// Check bottom right corner
	if (Collision::ABB(Vector2D(Game::hero.collider->collider.x + Game::hero.collider->collider.w, Game::hero.collider->collider.y + Game::hero.collider->collider.h), cCol))
		cp[7] = true;
}