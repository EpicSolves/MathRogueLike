#include "KeyboardController.h"

void KeyboardController::init()  {
	transform = &entity->getComponent<TransformComponent>();
	sprite = &entity->getComponent<SpriteComponent>();
}

void KeyboardController::update() {
	
	// Check if we need to exit the game
	if (Game::keyState[SDL_SCANCODE_ESCAPE]) {
		Game::isRunning = false;
		return;
	}

	// See if the user input a dash
	if (Game::keyState[SDL_SCANCODE_SPACE]) {
		if (!Game::dashCooldown) {
			Game::dashCooldown = true;
			Game::dashDuration = 20;
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

	// We do not want to be able to shoot during the math phase
	if (Game::keyState[SDL_SCANCODE_LCTRL] || 
		Game::phase == Game::MATH_PHASE_0  || 
		Game::phase == Game::MATH_PHASE_1  || 
		Game::phase == Game::MATH_PHASE_2)
		return;
	 
	// We can still solve during the fighting phase, but it is optional
	if (Game::keyState[SDL_SCANCODE_LEFT] || Game::keyState[SDL_SCANCODE_KP_4] || Game::keyState[SDL_SCANCODE_H]) {
		if (!leftDebounce) {
			leftDebounce = true;
			float x = transform->position.x + 58 * transform->scale;
			float y = transform->position.y + 52 * transform->scale;
			float xVel = -1.0f;
			float yVel = transform->velocity.y / 2.0f;

			Game::assets->CreateProjectile(Vector2D(x, y), Vector2D(xVel, yVel), 400, Game::hero.shotSpeed, "star", Game::groupPlayerProjectiles);
			Game::assets->PlaySound("star_shot");
		}
	}
	else leftDebounce = false;
	if (Game::keyState[SDL_SCANCODE_RIGHT] || Game::keyState[SDL_SCANCODE_KP_6] || Game::keyState[SDL_SCANCODE_L]) {
		if (!rightDebounce) {
			rightDebounce = true;
			float x = transform->position.x + 58 * transform->scale;
			float y = transform->position.y + 52 * transform->scale;
			Game::assets->CreateProjectile(Vector2D(x, y), Vector2D(1, transform->velocity.y / 2.0f), 400, Game::hero.shotSpeed, "star", Game::groupPlayerProjectiles);
			Game::assets->PlaySound("star_shot");
		}
	}
	else rightDebounce = false;
	if (Game::keyState[SDL_SCANCODE_DOWN] || Game::keyState[SDL_SCANCODE_KP_5] || Game::keyState[SDL_SCANCODE_J]) {
		if (!downDebounce) {
			downDebounce = true;
			float x = transform->position.x + 58 * transform->scale;
			float y = transform->position.y + 52 * transform->scale;
			Game::assets->CreateProjectile(Vector2D(x, y), Vector2D(transform->velocity.x / 2.0f, 1), 600, Game::hero.shotSpeed, "star", Game::groupPlayerProjectiles);
			Game::assets->PlaySound("star_shot");
		}
	}
	else downDebounce = false;
	if (Game::keyState[SDL_SCANCODE_UP] || Game::keyState[SDL_SCANCODE_KP_8] || Game::keyState[SDL_SCANCODE_K]) {
		if (!upDebounce) {
			upDebounce = true;
			float x = transform->position.x + 58 * transform->scale;
			float y = transform->position.y + 52 * transform->scale;
			Game::assets->CreateProjectile(Vector2D(x, y), Vector2D(transform->velocity.x / 2.0f, -1), 600, Game::hero.shotSpeed, "star", Game::groupPlayerProjectiles);
			Game::assets->PlaySound("star_shot");
		}
	}
	else upDebounce = false;
}
