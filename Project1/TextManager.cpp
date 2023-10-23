#include "TextManager.h"

void TextManager::Initialize()
{
	if (TTF_Init() < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not init TTF.");
		return;
	}

	font = TTF_OpenFont(font_file.c_str(), size);

	if (font == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not load TTF file.");
	}
}

void TextManager::DrawText(SDL_Renderer* renderer, const std::string& text, const SDL_Point& position)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Point size;
	SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
	const SDL_Rect text_rect{ position.x, position.y, size.x, size.y };

	SDL_RenderCopy(renderer, texture, nullptr, &text_rect);

	SDL_DestroyTexture(texture);
}

void TextManager::Destroy()
{
	TTF_CloseFont(font);
	font = nullptr;
}