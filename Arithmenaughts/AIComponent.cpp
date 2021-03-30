#include "AIComponent.h"

void AIComponent::init() {
	// Force a failure if no Collider Component
	collider = &entity->getComponent<ColliderComponent>();
	transform = &entity->getComponent<TransformComponent>();
}

void AIComponent::update() {

	// Calculate center of player and enemy
	float enemyX = collider->collider.x + collider->collider.w / 2.0f;
	float enemyY = collider->collider.y + collider->collider.h / 2.0f;

	float playerX = Game::player.getComponent<ColliderComponent>().collider.x +
		            Game::player.getComponent<ColliderComponent>().collider.w / 2.0f;
	float playerY = Game::player.getComponent<ColliderComponent>().collider.y +
		            Game::player.getComponent<ColliderComponent>().collider.h / 2.0f;

	// Determine if we should fire a shot
	if (canShoot) {
		if (shootTimer-- <= 0) {

			// Calculate x and y velocity
			float xDist = playerX - enemyX;
			float yDist = enemyY - playerY;
			float angle = std::atan2(yDist, xDist);
			float xVel = std::cos(angle);
			float yVel = std::sin(angle);

			// Create a little explosion to start the game	
			Game::assets->CreateProjectile(Vector2D(enemyX, enemyY), Vector2D(xVel, -yVel), 600, 4, "star", Game::groupEnemyProjectiles);
			shootTimer = (rand() % 3 + 3) * 60;
		}
	}

	// Ability is inactive
	abilityTimer--;
	if (!abilityIsActive) {
		if (abilityTimer <= 0) {
			abilityTimer = 30;
			abilityIsActive = true;
			randA = rand() % 1000 / 0.123;
			randB = rand() % 1000 / 0.123;
		}
	}
	else {
		if (abilityTimer <= 0) {
			abilityIsActive = false;
			abilityTimer = 90;
		}
	}

	if (abilityIsActive) {

		// Pick random direction
		float xVel = std::cos(randA+randB);
		float yVel = std::sin(randA+randB);

		// Create a little explosion to start the game	
		transform->velocity = Vector2D(xVel / 1.0f, -yVel / 1.0f);
	}
	else {

		// Calculate x and y velocity
		float xDist = playerX - enemyX;
		float yDist = enemyY - playerY;
		float angle = std::atan2(yDist, xDist);
		float xVel = std::cos(angle);
		float yVel = std::sin(angle);

		// Create a little explosion to start the game	
		transform->velocity = Vector2D(xVel / 3.0f, -yVel / 3.0f);
	}
}