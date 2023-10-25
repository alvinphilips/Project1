#pragma once

#ifndef PREFAB_MANAGER_H
#define PREFAB_MANAGER_H

#include <map>
#include <string>

#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"

class PrefabManager {
public:
	TextureManager *texture = nullptr;
	Player player;
	std::map<std::string, Enemy> enemies;
	std::map<std::string, Enemy> asteroids;
	std::map<std::string, Projectile> projectiles;
	void SetTextureManager(TextureManager* _texture) {
		texture = _texture;
	}
	void Destroy() {
		texture = nullptr;
	}
};


template<>
inline void load_from_json(PrefabManager& value, const json::JSON& node) {
	if (node.hasKey("player")) {
		load_from_json(value.player, node.at("player"));
		value.player.sprite = value.texture->GetSpriteIdByName(node.at("player").at("sprite").ToString());
		if (node.at("player").hasKey("projectile")) {
			Projectile p;
			load_from_json(p, node.at("player").at("projectile"));
			value.projectiles["player"] = p;
		}
	}
	if (node.hasKey("enemies")) {
		for (const auto& enemy : node.at("enemies").ArrayRange()) {
			Enemy e;
			e.sprite = value.texture->GetSpriteIdByName(enemy.at("sprite").ToString());
			load_from_json(e, enemy);
			std::string enemy_name = enemy.at("name").ToString();
			value.enemies[enemy_name] = e;
			if (enemy.hasKey("projectile")) {
				Projectile p;
				load_from_json(p, enemy.at("projectile"));
				value.projectiles[enemy_name] = p;
			}
		}
	}
	if (node.hasKey("asteroids")) {
		for (const auto& asteroid : node.at("asteroids").ArrayRange()) {
			Enemy e;
			e.sprite = value.texture->GetSpriteIdByName(asteroid.at("sprite").ToString());
			load_from_json(e, asteroid);
			value.asteroids[asteroid.at("name").ToString()] = e;
		}
	}
}
#endif