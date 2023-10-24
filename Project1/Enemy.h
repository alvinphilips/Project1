#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#include "Object.h"

enum EnemyBehaviour
{
	ENEMY_SLOW,
	ENEMY_RANDOM,
};

class Enemy final : public Object
{
public:
	float fire_delay;
	float last_fired;
	float speed;
	// Misc data
	unsigned int data;
	bool can_fire;

	void Update(const float deltaTime) override
	{
		Object::Update(deltaTime);
		last_fired += deltaTime;

		if (last_fired >= fire_delay)
		{
			can_fire = true;
			last_fired = 0;
		}
	}
	void Initialize() override
	{
		last_fired = 0;
		can_fire = true;
	}
	void Destroy() override {}
};

template <>
inline void load_from_json(Enemy& value, const json::JSON& node)
{
	load_from_json((Object&)value, node);
	if (node.hasKey("fire_delay"))
	{
		load_from_json(value.fire_delay, node.at("fire_delay"));
	}
	if (node.hasKey("data"))
	{
		load_from_json(value.data, node.at("data"));
	}
}

#endif