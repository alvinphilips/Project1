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
	TextureManager* texture = nullptr;
	Player player;
	Projectile player_projectile;
	std::map<std::string, Enemy> enemies;
	std::map<std::string, Enemy> asteroids;
	std::map<std::string, Projectile> projectiles;

	// Set the Texture Manager pointer, needs to be set before loading.
	void SetTextureManager(TextureManager* _texture) {
		texture = _texture;
	}
	// Reset texture pointer and clear storage
	void Destroy() {
		enemies.clear();
		asteroids.clear();
		projectiles.clear();
		texture = nullptr;
	}
};


template<>
inline void load_from_json(PrefabManager& value, const json::JSON& node) {
	// Warn if TextureManager was not set
	if (value.texture == nullptr) {
		std::cout << "WARNING: PrefabManager's texture field not set. Sprite IDs will not be set." << std::endl;
	}

	// Load Player
	if (node.hasKey("player")) {
		// Load the Player's data
		load_from_json(value.player, node.at("player"));

		// Set the Player's sprite
		if (value.texture != nullptr) {
			value.player.sprite = value.texture->GetSpriteIdByName(node.at("player").at("sprite").ToString());
		}

		// Add Player's projectile
		if (node.at("player").hasKey("projectile")) {
			load_from_json(value.player_projectile, node.at("player").at("projectile"));
		}
	}

	// Load enemies
	if (node.hasKey("enemies")) {
		for (const auto& enemy : node.at("enemies").ArrayRange()) {
			Enemy e;
			// Set the enemy's sprite
			if (value.texture != nullptr) {
				e.sprite = value.texture->GetSpriteIdByName(enemy.at("sprite").ToString());
			}

			// Load the enemy's data
			load_from_json(e, enemy);

			// Load the enemy's name, and use it as a key in our map of Enemies
			std::string enemy_name = enemy.at("name").ToString();
			value.enemies[enemy_name] = e;

			// Add enemy's projectile
			if (enemy.hasKey("projectile")) {
				Projectile p;
				load_from_json(p, enemy.at("projectile"));
				value.projectiles[enemy_name] = p;
			}
		}
	}

	// Load asteroids
	if (node.hasKey("asteroids")) {
		for (const auto& asteroid : node.at("asteroids").ArrayRange()) {
			Enemy e;

			// Set the asteroid's sprite
			if (value.texture != nullptr) {
			e.sprite = value.texture->GetSpriteIdByName(asteroid.at("sprite").ToString());
			}

			// Load the asteroid's data
			load_from_json(e, asteroid);

			// Load the asteroid's name, and use it as a key in our map of Asteroids (just Enemies)
			value.asteroids[asteroid.at("name").ToString()] = e;
		}
	}
}
#endif