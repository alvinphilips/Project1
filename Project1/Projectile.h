#pragma once

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Object.h"

class Projectile final : public Object {
public:
	unsigned int damage;
	float scale = 1.0f;
	void Initialize() override {}
	void Damage(Object& object) const {
		object.TakeDamage(damage);
	}
	void Destroy() override {}
};

template <>
inline void load_from_json(Projectile& value, const json::JSON& node) {
	load_from_json((Object&)value, node);
	if (node.hasKey("damage")) {
		load_from_json(value.damage, node.at("damage"));
	}
	if (node.hasKey("scale")) {
		load_from_json(value.scale, node.at("scale"));
	}
}

#endif