#pragma once

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include <SDL/SDL_ttf.h>
#include <string>

#include "Serialize.h"

class TextManager
{
public:
	void Initialize()
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

	// Write text to the top left corner of the screen
	void DrawText(SDL_Renderer* renderer, const std::string& text)
	{
		DrawText(renderer, text, { 0, 0 });
	}

	// Write text to a specified location on the screen
	void DrawText(SDL_Renderer* renderer, const std::string& text, const SDL_Point& position)
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

	void Destroy()
	{
		TTF_CloseFont(font);
		font = nullptr;
	}
private:
	TTF_Font* font = nullptr;
public:
	std::string font_file;
	SDL_Color color{ 255, 255, 255, 255 };
	int size = 12;
};

template<>
inline void load_from_json(TextManager& value, const json::JSON& node)
{
	if (node.hasKey("font"))
	{
		load_from_json(value.font_file, node.at("font"));
	}

	if (node.hasKey("color"))
	{
		load_from_json(value.color, node.at("color"));
	}

	if (node.hasKey("size"))
	{
		load_from_json(value.size, node.at("size"));
	}
}
#endif