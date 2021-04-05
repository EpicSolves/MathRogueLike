#pragma once

#include "AssetManager.h"
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include <string>

class AssetManager;
class Animation;

class NameplateComponent : public Component {

private:
	TransformComponent *transform;
	SDL_Texture *texture;
	StatsComponent *stats;
public:

	SDL_Rect sRect, dRect;

	NameplateComponent();

	NameplateComponent(std::string texId);

	~NameplateComponent();

	void setTex(std::string texId);

	void init() override;

	void update() override;

	void draw() override;
};