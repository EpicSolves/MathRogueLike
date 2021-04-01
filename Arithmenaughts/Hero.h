#pragma once
#include "ClassList.h"
#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include <map>
#include <vector>

class Hero {
public:
	Hero();
	Entity& entity;

	// Basic components needed to control and see character
	TransformComponent *transform;
	SpriteComponent *sprite;
	KeyboardController *controller;
	ColliderComponent *collider;

	// Bow components
	SpriteComponent *bowSprite;
	TransformComponent *bowTransform;

	// Visible stats
	float strength = 5.0;
	float agility = 5.0;
	float critical = 5.0f;
	float stamina = 5.0f;
	float hitPoints = 100.0f*(1.0f + stamina / 500.0f);

	// Underlying stats
	float shotSpeed = agility;
	float damageIncreaseCoeff = 1.0f + (agility + strength) / 1000.0f;
	float damageReductionCoeff = 1.0f + stamina / 500.0f;
	float shotCooldown;
	float damagerPerShot = (strength*3.0 + agility)*damageIncreaseCoeff;

	// Boolean for flagging when stats change
	bool statsChanged = true;

	// Added stats from gear
	int bowStrength = 0;
	int bowAgility = 0;
	int bowCritical = 0;
	int bowStamina = 0;

	// Added stats from solving
	float solveStrengthBonus = 0.0f;
	float solveAgilityBonus = 0.0f;
	float solveCriticalBonus = 0.0f;
	float solveStaminaBonus = 0.0f;

	// Functions
	void init();
	void setBow(std::string color);
	void calculateStats();
	bool didCrit();
private:
};