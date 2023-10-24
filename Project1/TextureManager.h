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
	void Initialize(SDL_Renderer *renderer)
	{
		SDL_Surface* surface = IMG_Load(atlas_file.c_str());
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	// Returns the ID of a Sprite, or -1 if it was not found
	int GetSpriteIdByName(const std::string& name) {
		if (sprite_lookup_table.find(name) != sprite_lookup_table.end())
		{
			return (int) sprite_lookup_table.at(name);
		}
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Cannot find sprite with name '%s'", name.c_str());
		return -1;
	}

	// Returns a Sprite at a given position in the sprite array, or the first element if the index is out of range
	Sprite GetSpriteById(const unsigned int id)
	{
		if (id >= sprite_storage.size())
		{
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Cannot find sprite with id '%ui'", id);

			// Hopefully we have at least one Sprite stored
			return sprite_storage[0];
		}
		return sprite_storage[id];
	}
	void Destroy()
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}
};

template <>
inline void load_from_json(TextureManager& value, const json::JSON& node)
{
	// We currently only care about and support a texture atlas
	if (!node.hasKey("atlas"))
	{
		SDL_LogInfo(SDL_LOG_CATEGORY_ERROR, "Could not load a texture atlas.");
		return;
	}

	const json::JSON& atlas = node.at("atlas");
	if (atlas.hasKey("image_path")) 
	{
		load_from_json(value.atlas_file, atlas.at("image_path"));
	}

	if (!atlas.hasKey("subtextures"))
	{
		SDL_LogInfo(SDL_LOG_CATEGORY_ERROR, "Atlas does not define any subtextures.");
		return;
	}

	value.sprite_storage.reserve(atlas.at("subtextures").length());

	// Add all Subtextures
	for (auto& subtexture: atlas.at("subtextures").ArrayRange())
	{
		Sprite sprite;
		load_from_json(sprite, subtexture);
		value.sprite_storage.push_back(sprite);
		std::string sprite_name = subtexture.at("name").ToString();
		value.sprite_lookup_table[sprite_name] = (unsigned int) value.sprite_storage.size() - 1;
	}
}

#endif
