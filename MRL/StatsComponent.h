#pragma once

#include "ECS.h"
#include <map>
#include <string>
#include "Components.h"

class StatsComponent : public Component {
public:

	StatsComponent();
	void init() override;
	void update() override;
	float health;
};