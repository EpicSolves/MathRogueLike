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

	float playerX = Game::hero.collider->collider.x +
				    Game::hero.collider->collider.w / 2.0f;
	float playerY = Game::hero.collider->collider.y +
		            Game::hero.collider->collider.h / 2.0f;

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
			std::uniform_int_distribution<> y(180, 360);
			shootTimer = y(Game::rng);
		}
	}

	// Ability is inactive
	abilityTimer--;
	if (!abilityIsActive) {
		if (abilityTimer <= 0) {
			std::uniform_int_distribution<> d(120, 240);
			abilityTimer = d(Game::rng);
			abilityIsActive = true;
			std::uniform_real_distribution<double> x(0, 2 * 3.14159);
			randA = x(Game::rng);
			randB = x(Game::rng);
		}
	}
	else {
		if (abilityTimer <= 0) {
			std::uniform_int_distribution<> d(720, 1000);
			abilityIsActive = false;
			abilityTimer = d(Game::rng);
		}
	}

	if (abilityIsActive) {

		// Pick random direction
		// float xVel = std::cos(randA+randB);
		// float yVel = std::sin(randA+randB);
		float xDist = playerX - enemyX;
		float yDist = enemyY - playerY;
		float angle = std::atan2(yDist, xDist);
		float xVel = std::cos(angle);
		float yVel = std::sin(angle);

		// Create a little explosion to start the game	
		transform->velocity = Vector2D(xVel / 1.5f, -yVel / 1.5f);
	}
	else {

		// Calculate x and y velocity
		float xDist = playerX - enemyX;
		float yDist = enemyY - playerY;
		float angle = std::atan2(yDist, xDist);
		float xVel = std::cos(angle);
		float yVel = std::sin(angle);
		transform->velocity = Vector2D(xVel / 4.5f, -yVel / 4.5f);
	}
}