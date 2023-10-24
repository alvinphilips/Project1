#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include <SDL/SDL_rect.h>

#include "Serialize.h"
#include "TextureManager.h"

// An Object defines the base set of functionality an entity in the game possesses
class Object
{
public:
	SDL_FPoint pos;
	SDL_FPoint vel;
	unsigned int radius;
	unsigned int sprite;
	unsigned int health;
	bool is_dead;

	Object() : pos{ 0, 0 }, vel{ 0, 0 } {
		radius = 0;
		sprite = 0;
		health = 100;
		is_dead = false;
	}
	virtual ~Object() = default;
	virtual void Update(const float deltaTime)
	{
		// Move the object based on its velocity
		pos.x += vel.x * deltaTime;
		pos.y += vel.y * deltaTime;
	}
	void TakeDamage(const unsigned int damage)
	{
		health -= SDL_min(damage, health);
		if (health == 0)
		{
			is_dead = true;
		}
	}
	virtual void Initialize() = 0;
	virtual void Destroy() = 0;
	void SetSprite(TextureManager& texture, const std::string& name)
	{
		sprite = texture.GetSpriteIdByName(name);
		// We expect square, or square-ish Sprites
		radius = texture.GetSpriteById(sprite).bounds.w / 2;
	}
	void SetSprite(TextureManager& texture, const unsigned int id)
	{
		sprite = id;
		// We expect square, or square-ish Sprites
		radius = texture.GetSpriteById(sprite).bounds.w / 2;
	}
};

template <>
inline void load_from_json(Object& value, const json::JSON& node)
{
	if (node.hasKey("pos"))
	{
		load_from_json(value.pos, node.at("pos"));
	}
	if (node.hasKey("vel"))
	{
		load_from_json(value.vel, node.at("vel"));
	}
	if (node.hasKey("radius")) {
		load_from_json(value.radius, node.at("radius"));
	}
	if (node.hasKey("health")) {
		load_from_json(value.health, node.at("health"));
	}
}

#endif