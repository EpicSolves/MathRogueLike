#pragma once

#include "ECS.h"
#include <map>
#include <string>

class StatsComponent : public Component {
public:

	std::map<std::string, float> resources;
	std::string trackTag;
	TransformComponent *transform;
	StatsComponent() {
		trackTag = "default";
		resources[trackTag] = 100.0f;
	}
	StatsComponent(std::string initialResource, float value) {
		resources[initialResource] = value;
		trackTag = initialResource;
	}

	void setResource(std::string tag, float value) {
		resources[tag] = value;
	}

	float getResource(std::string tag) {
		return resources[tag];
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		transform->width = (int)resources[trackTag];
	}
};