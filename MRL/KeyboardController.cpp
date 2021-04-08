#include "KeyboardController.h"

void KeyboardController::init()  {
	transform = &entity->getComponent<TransformComponent>();
	sprite = &entity->getComponent<SpriteComponent>();
}

void KeyboardController::update() {

	// Update timers
	shotTimer = std::max(0, shotTimer - 1);
	Game::hero.frostShotTimer = std::max(0, Game::hero.frostShotTimer - 1);
	
	// Check if we need to exit the game
	if (Game::keyState[SDL_SCANCODE_ESCAPE]) {
		Game::isRunning = false;
		return;
	}

	// See if the user input a dash
	if (Game::keyState[SDL_SCANCODE_SPACE]) {
		if (!Game::dashCooldown && Game::hero.energy >= 20) {
			Game::dashCooldown = true;

			Game::dashDuration = 20;
			Game::hero.energy -= 33;
			sprite->currentFrame = 0;
		}
		else {
			Game::dashDuration--;
		}
	}
	else {
		Game::dashDuration--;
	}
	if (Game::dashDuration <= 0) {
		Game::dashCooldown = false;
	}

	// WASD controls
	if (!Game::dashCooldown) {
		if (Game::keyState[SDL_SCANCODE_W] && Game::keyState[SDL_SCANCODE_S])
			transform->velocity.y = 0;
		else if (Game::keyState[SDL_SCANCODE_W])
			transform->velocity.y = -1;
		else if (Game::keyState[SDL_SCANCODE_S])
			transform->velocity.y = 1;
		else
			transform->velocity.y = 0;

		if (Game::keyState[SDL_SCANCODE_A] && Game::keyState[SDL_SCANCODE_D])
			transform->velocity.x = 0;
		else if (Game::keyState[SDL_SCANCODE_A])
			transform->velocity.x = -1;
		else if (Game::keyState[SDL_SCANCODE_D])
			transform->velocity.x = 1;
		else
			transform->velocity.x = 0;

		// Normalize speed based on velocity
		if (transform->velocity.y != 0 && transform->velocity.x != 0) {
			if (Game::keyState[SDL_SCANCODE_LSHIFT])
				transform->speed = 7.0f / 1.3f;
			else
				transform->speed = 5.0f / 1.3f;
		}
		else {
			if (Game::keyState[SDL_SCANCODE_LSHIFT])
				transform->speed = 7.0f;
			else
				transform->speed = 5.0f;
		}
	}
	else {
		if (transform->velocity.y != 0 && transform->velocity.x != 0)
			transform->speed = 9.0f/1.3f;
		else
			transform->speed = 9.0f;
	}

	// Animating the player
	if (transform->velocity.x == 0 && transform->velocity.y == 0) {
		Game::hero.bowSprite->Play("player_idle");
		sprite->Play("player_idle");
	}
	else if (Game::dashCooldown) {
		sprite->Play("player_dash");
	}
	else {
		Game::hero.bowSprite->Play("player_run");
		sprite->Play("player_run");
	}

	if (transform->velocity.x > 0) {
		sprite->spriteFlip = SDL_FLIP_NONE;
	}
	else if (transform->velocity.x < 0) {
		sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
	}
	 
	// Enable shooting if not in math phase
	if (Game::mouseState & SDL_BUTTON(1)) {
		if (shotTimer == 0) {
			float playerX = Game::hero.collider->collider.x +
				            Game::hero.collider->collider.w / 2.0f;
			float playerY = Game::hero.collider->collider.y +
				            Game::hero.collider->collider.h / 2.0f;


			float xDist = (Game::mouseX + Game::camera.x - 8.0f) - playerX;
			float yDist = playerY - (Game::mouseY + Game::camera.y - 8.0f);
			float angle = std::atan2(yDist, xDist);
			float xVel = std::cos(angle);
			float yVel = std::sin(angle);

			Game::assets->CreateProjectile(Vector2D(playerX-32.0f, playerY-40.0f), Vector2D(xVel, -yVel), 400, Game::hero.shotSpeed, "normal_arrow", Game::groupPlayerProjectiles, angle);
			Game::assets->PlaySound("star_shot");
			shotTimer = Game::hero.shotCooldown;
		}
	}
	// Enable shooting if not in math phase
	if (Game::mouseState & SDL_BUTTON(3)) {
		printf("Hello\n");
		if (Game::hero.frostShotTimer == 0) {
			float playerX = Game::hero.collider->collider.x +
				Game::hero.collider->collider.w / 2.0f;
			float playerY = Game::hero.collider->collider.y +
				Game::hero.collider->collider.h / 2.0f;


			float xDist = (Game::mouseX + Game::camera.x - 8.0f) - playerX;
			float yDist = playerY - (Game::mouseY + Game::camera.y - 8.0f);
			float angle = std::atan2(yDist, xDist);
			float xVel = std::cos(angle);
			float yVel = std::sin(angle);

			Game::assets->CreateProjectile(Vector2D(playerX - 32.0f, playerY - 40.0f), Vector2D(xVel, -yVel), 400, Game::hero.shotSpeed, "fire_arrow", Game::groupFireProjectiles, angle);
			Game::assets->PlaySound("star_shot");
			Game::hero.frostShotTimer = Game::hero.frostShotCooldown;
		}
	}
	if (Game::phase != Game::MATH_PHASE) {
		if (Game::keyState[SDL_SCANCODE_LEFT] || Game::keyState[SDL_SCANCODE_KP_4] || Game::keyState[SDL_SCANCODE_H]) {
			if (!leftDebounce) {
				if (shotTimer == 0) {
					leftDebounce = true;

					float x = transform->position.x + 58 * transform->scale;
					float y = transform->position.y + 52 * transform->scale;
					float xVel = -1.0f;
					float yVel = transform->velocity.y / 2.0f;

					Game::assets->CreateProjectile(Vector2D(x, y), Vector2D(xVel, yVel), 400, Game::hero.shotSpeed, "star_spin", Game::groupPlayerProjectiles);
					Game::assets->PlaySound("star_shot");
					shotTimer = Game::hero.shotCooldown;
				}
			}
		}
		else leftDebounce = false;
		if (Game::keyState[SDL_SCANCODE_RIGHT] || Game::keyState[SDL_SCANCODE_KP_6] || Game::keyState[SDL_SCANCODE_L]) {
			if (!rightDebounce) {
				if (shotTimer == 0) {
					rightDebounce = true;
					float x = transform->position.x + 58 * transform->scale;
					float y = transform->position.y + 52 * transform->scale;
					Game::assets->CreateProjectile(Vector2D(x, y), Vector2D(1, transform->velocity.y / 2.0f), 400, Game::hero.shotSpeed, "star_spin", Game::groupPlayerProjectiles);
					Game::assets->PlaySound("star_shot");
					shotTimer = Game::hero.shotCooldown;
				}
			}
		}
		else rightDebounce = false;
		if (Game::keyState[SDL_SCANCODE_DOWN] || Game::keyState[SDL_SCANCODE_KP_5] || Game::keyState[SDL_SCANCODE_J]) {
			if (!downDebounce) {
				if (shotTimer == 0) {
					downDebounce = true;
					float x = transform->position.x + 58 * transform->scale;
					float y = transform->position.y + 52 * transform->scale;
					Game::assets->CreateProjectile(Vector2D(x, y), Vector2D(transform->velocity.x / 2.0f, 1), 600, Game::hero.shotSpeed, "star_spin", Game::groupPlayerProjectiles);
					Game::assets->PlaySound("star_shot");
					shotTimer = Game::hero.shotCooldown;
				}
			}
		}
		else downDebounce = false;
		if (Game::keyState[SDL_SCANCODE_UP] || Game::keyState[SDL_SCANCODE_KP_8] || Game::keyState[SDL_SCANCODE_K]) {
			if (!upDebounce) {
				if (shotTimer == 0) {
					upDebounce = true;
					float x = transform->position.x + 58 * transform->scale;
					float y = transform->position.y + 52 * transform->scale;
					Game::assets->CreateProjectile(Vector2D(x, y), Vector2D(transform->velocity.x / 2.0f, -1), 600, Game::hero.shotSpeed, "star_spin", Game::groupPlayerProjectiles);
					Game::assets->PlaySound("star_shot");
					shotTimer = Game::hero.shotCooldown;
				}
			}
		}
		else upDebounce = false;
	}
}
