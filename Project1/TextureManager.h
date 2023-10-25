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
public:
	SDL_Texture* texture = nullptr;
	// Map of Sprite names to their indices in sprite_storage
	std::map<std::string, unsigned int> sprite_lookup_table;
	// Array of actual sprite data storage
	std::vector<Sprite> sprite_storage;
	// Path to the image file
	std::string atlas_file;
	void Initialize(SDL_Renderer* renderer);
	// Returns the ID of a Sprite, or -1 if it was not found
	int GetSpriteIdByName(const std::string& name);

	// Returns a Sprite at a given position in the sprite array, or the first element if the index is out of range
	Sprite GetSpriteById(unsigned int id);
	void Destroy();
};

template <>
void load_from_json(TextureManager&, const json::JSON&);

#endif
