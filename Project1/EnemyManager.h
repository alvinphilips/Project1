#pragma once

#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <vector>

#include "Enemy.h"
#include "Player.h"
#include "ProjectileManager.h"

class EnemyManager {
	void ResetEnemy(Enemy& enemy) {
		enemy.Initialize();
		if (window_size != nullptr) {
			// Cast to float to get a float position, rather than a int
			enemy.pos.x = random(0.0f, (float)window_size->x) * 2.0f;
			// Random position above the screen
			enemy.pos.y = (float)random(-500, -100);
		}
	}
	SDL_Point* window_size = nullptr;
	Player* player = nullptr;
public:
	void ResetAllEnemies() {
		for (auto& enemy : enemies) {
			ResetEnemy(enemy);
		}
	}
	std::vector<Enemy> enemies;
	bool can_collide = true;
	ProjectileManager bullets;

	void Initialize();
	void SetWindowSizeAndPlayer(SDL_Point* _window_size, Player* _player);
	void AddEnemy(const Enemy& prefab);
	unsigned int UpdateAndGetScore(const float deltaTime, ProjectileManager& player_bullets);
	void DrawEnemies(SDL_Renderer* renderer, TextureManager& texture);
	void Destroy();
};

template <>
inline void load_from_json(EnemyManager& value, const json::JSON& node) {
	// Clear current enemy list
	value.enemies.clear();

	if (node.hasKey("can_collide")) {
		load_from_json(value.can_collide, node.at("can_collide"));
	}

	if (node.hasKey("enemies")) {
		for (auto& enemy : node.at("enemies").ArrayRange()) {
			Enemy e;
			load_from_json(e, enemy);
			value.enemies.push_back(e);
		}
	}
}

template <>
inline json::JSON save_to_json(const EnemyManager& value) {
	json::JSON node;
	node["can_collide"] = value.can_collide;
	node["enemies"] = json::JSON::Array();
	for (auto& enemy : value.enemies) {
		node["enemies"].append(save_to_json(enemy));
	}
	return node;
}

#endif