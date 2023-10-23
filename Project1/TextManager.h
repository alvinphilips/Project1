#pragma once

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include <SDL/SDL_ttf.h>
#include <string>

#include "Serialize.h"

class TextManager
{
public:
	void Initialize();

	// Write text to the top left corner of the screen
	void DrawText(SDL_Renderer* renderer, const std::string& text)
	{
		DrawText(renderer, text, { 0, 0 });
	}

	// Write text to a specified location on the screen
	void DrawText(SDL_Renderer*, const std::string&, const SDL_Point&);

	void Destroy();
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