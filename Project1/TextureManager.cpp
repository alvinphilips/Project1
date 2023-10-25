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

template <>
void load_from_json(TextureManager& texture, const json::JSON& node)
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
		load_from_json(texture.atlas_file, atlas.at("image_path"));
	}

	if (!atlas.hasKey("subtextures"))
	{
		SDL_LogInfo(SDL_LOG_CATEGORY_ERROR, "Atlas does not define any subtextures.");
		return;
	}

	texture.sprite_storage.reserve(atlas.at("subtextures").length());

	// Add all Subtextures
	for (auto& subtexture: atlas.at("subtextures").ArrayRange())
	{
		Sprite sprite;
		load_from_json(sprite, subtexture);
		texture.sprite_storage.push_back(sprite);
		std::string sprite_name = subtexture.at("name").ToString();
		texture.sprite_lookup_table[sprite_name] = (unsigned int) texture.sprite_storage.size() - 1;
	}
}