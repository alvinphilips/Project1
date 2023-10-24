#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Object.h"

class Player: public Object
{
public:
	float fire_delay;
	float last_fired;
	float speed;
	float movement_factor;
	float velocity_dampener;
	unsigned int lives;
	bool can_fire;
	void Initialize() override {}
	void Destroy() override {}
};

template <>
inline void load_from_json(Player& value, const json::JSON& node)
{
	load_from_json((Object&)value, node);
}

#endif