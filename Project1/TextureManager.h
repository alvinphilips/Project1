#pragma once

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL/SDL_render.h>
#include <SDL/SDL_image.h>

#include <map>
#include <vector>
#include <string>

#include "Sprite.h"

class TextureManager
{
	std::map<std::string, unsigned int> sprite_lookup_table;
	std::vector<Sprite> sprite_storage;
	SDL_Texture* texture = nullptr;
public:
	std::string atlas_file;
	void Initialize(SDL_Renderer *renderer)
	{
		SDL_Surface* surface = IMG_Load(atlas_file.c_str());
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	Sprite GetSpriteByName(std::string& name)
	{
		// TODO: implement
		return sprite_storage[0];
	}
	Sprite GetSpriteById(unsigned int id)
	{
		// TODO: implement
		return sprite_storage[0];
	}
	void Destroy()
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
};

#endif
