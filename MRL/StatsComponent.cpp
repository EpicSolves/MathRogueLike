#include "StatsComponent.h"

StatsComponent::StatsComponent() {
	trackTag = "default";
	resources[trackTag] = 100.0f;
}
StatsComponent::StatsComponent(std::string initialResource, float value) {
	resources[initialResource] = value;
	trackTag = initialResource;
}

void StatsComponent::setResource(std::string tag, float value) {
	resources[tag] = value;
}

float StatsComponent::getResource(std::string tag) {
	return resources[tag];
}

void StatsComponent::init() {
	transform = &entity->getComponent<TransformComponent>();
}

void StatsComponent::update() {
	transform->width = (int)resources[trackTag];
}