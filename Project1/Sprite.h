#pragma once

#ifndef SPRITE_H
#define SPRITE_H

#include <SDL/SDL_rect.h>

#include "Serialize.h"

struct Sprite
{
	SDL_Rect bounds;
};

template <>
inline void load_from_json(Sprite& value, const json::JSON& node)
{
	load_from_json(value.bounds, node);
}

#endif