#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include <SDL/SDL_rect.h>

#include "Serialize.h"

class Object
{
public:
	SDL_FPoint pos;
	SDL_FPoint vel;
	unsigned int radius;
	unsigned int sprite;
	virtual void Update(float deltaTime) = 0;
	virtual void Initialize() = 0;
	virtual void Destroy() = 0;
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
}

#endif