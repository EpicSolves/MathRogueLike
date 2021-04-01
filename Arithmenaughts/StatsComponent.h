#pragma once

#include "ECS.h"
#include <map>
#include <string>
#include "Components.h"

class StatsComponent : public Component {
public:

	std::map<std::string, float> resources;
	std::string trackTag;
	TransformComponent *transform;
	StatsComponent();
	StatsComponent(std::string initialResource, float value);

	void setResource(std::string tag, float value);

	float getResource(std::string tag);

	void init() override;

	void update() override;
};