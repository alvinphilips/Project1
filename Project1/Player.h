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
	
	if (node.hasKey("speed")) {
		load_from_json(value.speed, node.at("speed"));
	}
	if (node.hasKey("fire_delay")) {
		load_from_json(value.fire_delay, node.at("fire_delay"));
	}
	if (node.hasKey("last_fired")) {
		load_from_json(value.last_fired, node.at("last_fired"));
	}
	if (node.hasKey("movement_factor")) {
		load_from_json(value.movement_factor, node.at("movement_factor"));
	}
	if (node.hasKey("velocity_dampener")) {
		load_from_json(value.velocity_dampener, node.at("velocity_dampener"));
	}
	if (node.hasKey("lives")) {
		load_from_json(value.lives, node.at("lives"));
	}

}

#endif