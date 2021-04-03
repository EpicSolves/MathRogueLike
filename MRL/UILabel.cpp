#include "UILabel.h"

class AssetManager;
UILabel::UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color& colour, std::string tid) :
	labelText(text), labelFont(font), textColour(colour), tagId(tid)
{
	position.x = xpos;
	position.y = ypos;

	SetLabelText(labelText, labelFont);
}
UILabel::~UILabel()
{}

void UILabel::SetLabelText(std::string text, std::string font)
{
	SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont(font), text.c_str(), textColour);
	labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
	SDL_FreeSurface(surf);

	SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
}

void UILabel::draw()
{
	SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
}