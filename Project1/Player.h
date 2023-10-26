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
	void Update(const float deltaTime) override {
		Object::Update(deltaTime);
		last_fired += deltaTime;

		if (last_fired >= fire_delay)
		{
			can_fire = true;
			last_fired = 0;
		}
	}
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

template <>
inline json::JSON save_to_json(const Player& value)
{
	json::JSON node = save_to_json((Object&)value);
	node["fire_delay"] = value.fire_delay;
	node["movement_factor"] = value.movement_factor;
	node["speed"] = value.speed;
	node["can_fire"] = value.can_fire;
	node["velocity_dampener"] = value.velocity_dampener;
	node["lives"] = value.lives;

	return node;
}


#endif