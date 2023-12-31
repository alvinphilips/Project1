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
	std::map<std::string, Enemy> decorations;
	Projectile enemy_projectile;

	// Set the Texture Manager pointer, needs to be set before loading.
	void SetTextureManager(TextureManager* _texture) {
		texture = _texture;
	}
	// Reset texture pointer and clear storage
	void Destroy() {
		enemies.clear();
		decorations.clear();
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
			value.player.SetSprite(*value.texture, node.at("player").at("sprite").ToString());
		}

		// Add Player's projectile
		if (node.at("player").hasKey("projectile")) {
			load_from_json(value.player_projectile, node.at("player").at("projectile"));
			// Set the projectile's sprite
			if (value.texture != nullptr) {
				value.player_projectile.SetSprite(*value.texture, node.at("player").at("projectile").at("sprite").ToString());
			}
		}
	}

	// Add enemy's projectile
	if (node.hasKey("enemy_projectile")) {
		load_from_json(value.enemy_projectile, node.at("enemy_projectile"));

		if (value.texture != nullptr) {
			value.enemy_projectile.SetSprite(*value.texture,  node.at("enemy_projectile").at("sprite").ToString());
		}
	}

	// Load enemies
	if (node.hasKey("enemies")) {
		for (const auto& enemy : node.at("enemies").ArrayRange()) {
			Enemy e;
			// Set the enemy's sprite
			if (value.texture != nullptr) {
				e.SetSprite(*value.texture, enemy.at("sprite").ToString());
			}

			// Load the enemy's data
			load_from_json(e, enemy);

			// Load the enemy's name, and use it as a key in our map of Enemies
			value.enemies[enemy.at("name").ToString()] = e;
		}
	}

	// Load background elements
	if (node.hasKey("decorations")) {
		for (const auto& decoration : node.at("decorations").ArrayRange()) {
			Enemy e;

			// Set the decoration's sprite
			if (value.texture != nullptr) {
				e.SetSprite(*value.texture, decoration.at("sprite").ToString());
			}

			// Load the decoration's data
			load_from_json(e, decoration);

			// Load the decoration's name, and use it as a key in our map of background elements
			value.decorations[decoration.at("name").ToString()] = e;
		}
	}
}
#endif