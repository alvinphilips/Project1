#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#include "Object.h"
#include "Math.h"

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
	float max_health;
	// Scale the Enemy's sprite by a factor
	float scale = 1;
	// Maximum speed of the Enemy
	float speed;
	// How many points this Enemy is worth
	unsigned int points;
	// Does this Enemy have the ability to shoot Projectiles?
	bool can_fire;
	// Is the Enemy ready to shoot?
	bool shot_ready;

	void Update(const float deltaTime) override
	{
		Object::Update(deltaTime);
		last_fired += deltaTime;

		if (last_fired >= fire_delay)
		{
			shot_ready = can_fire;
			last_fired = 0;
		}
	}
	void Initialize() override
	{
		vel.x = random(-speed, speed);
		vel.y = random(speed);
		last_fired = 0;
		shot_ready = can_fire;
		health = max_health;
		is_dead = false;
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
	if (node.hasKey("points"))
	{
		load_from_json(value.points, node.at("points"));
	}
	if (node.hasKey("speed"))
	{
		load_from_json(value.speed, node.at("speed"));
	}
	if (node.hasKey("can_fire"))
	{
		load_from_json(value.can_fire, node.at("can_fire"));
	}
	if (node.hasKey("scale"))
	{
		load_from_json(value.scale, node.at("scale"));
	}
	value.max_health = value.health;
}

#endif