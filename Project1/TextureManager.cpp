#include "TextureManager.h"

void TextureManager::Initialize(SDL_Renderer* renderer)
{
	SDL_Surface* surface = IMG_Load(atlas_file.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

int TextureManager::GetSpriteIdByName(const std::string& name) {
	if (sprite_lookup_table.find(name) != sprite_lookup_table.end())
	{
		return (int)sprite_lookup_table.at(name);
	}
	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Cannot find sprite with name '%s'", name.c_str());
	return -1;
}

Sprite TextureManager::GetSpriteById(const unsigned int id)
{
	if (id >= sprite_storage.size())
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Cannot find sprite with id '%ui'", id);

		// Hopefully we have at least one Sprite stored
		return sprite_storage[0];
	}
	return sprite_storage[id];
}

void TextureManager::Destroy()
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
}