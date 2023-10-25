#pragma once

#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <vector>

#include "Enemy.h"
#include "Player.h"

class EnemyManager {
	std::vector<Enemy> enemies;
	void ResetEnemy(Enemy& enemy) {
		enemy.Initialize();
		if (window_size != nullptr) {
			// Cast to float to get a float position, rather than a int
			enemy.pos.x = random(0.0f, (float)window_size->x) * 2.0f;
			// Random position above the screen
			enemy.pos.y = (float) random(-200, -100);
		}
	}
	SDL_Point* window_size = nullptr;
	Player* player = nullptr;
public:
	bool can_collide = true;
	void Initialize() {
		for (auto& enemy : enemies) {
			ResetEnemy(enemy);
		}
	}
	void SetWindowSizeAndPlayer(SDL_Point* _window_size, Player* _player) {
		window_size = _window_size;
		player = _player;
	}
	void AddEnemy(const Enemy& prefab) {
		Enemy e = prefab;
		ResetEnemy(e);
		enemies.push_back(e);
	}
	unsigned int UpdateAndGetScore(const float deltaTime) {
		unsigned int score = 0;
		for (auto& enemy : enemies) {
			enemy.Update(deltaTime);

			if (enemy.pos.y > window_size->y || enemy.pos.x < -100 || enemy.pos.x > window_size->x + 100) {
				ResetEnemy(enemy);
			}

			// Calculate collisions
			if (can_collide && check_collision(player->pos, enemy.pos, player->radius, enemy.radius * enemy.scale)) {
				enemy.is_dead = true;
				player->is_dead = true;
			}

			// Reset dead Enemies
			if (enemy.is_dead) {
				score += enemy.points;
				ResetEnemy(enemy);
				enemy.is_dead = false;
			} else if (enemy.shot_ready) {
				// The dead can't shoot
				bullets.push_back(enemy.pos);
				enemy.shot_ready = false;
			}

		}

		return score;
	}
	void DrawEnemies(SDL_Renderer* renderer, TextureManager& texture) {
		for (auto& enemy : enemies) {
			SDL_Rect enemy_sprite = texture.sprite_storage[enemy.sprite].bounds;

			// Scale enemy by half
			SDL_FRect enemy_rect = {enemy.pos.x, enemy.pos.y, (float) enemy_sprite.w * enemy.scale, (float) enemy_sprite.h * enemy.scale};
			
			SDL_SetTextureColorMod(texture.texture, enemy.color.r, enemy.color.g, enemy.color.b);
			// SDL_RenderCopyF(renderer, texture.texture, &enemy_sprite, &enemy_rect);
			SDL_RenderCopyExF(renderer, texture.texture, &enemy_sprite, &enemy_rect, 0, nullptr, SDL_FLIP_VERTICAL);
			SDL_SetTextureColorMod(texture.texture, 255, 255, 255);
		}
	}
	void Destroy() {
		player = nullptr;
		window_size = nullptr;
		for (auto& enemy : enemies) {
			enemy.Destroy();
		}
		enemies.clear();
	}
};

#endif