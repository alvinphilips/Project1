#pragma once

#include <vector>
#include "Projectile.h"

class ProjectileManager {
public:
	Projectile p;
	std::vector<SDL_FPoint> projectiles;

	// Update Projectile positions
	void Update(const float deltaTime) {
		for (auto& projectile : projectiles) {
			projectile.x += p.vel.x * deltaTime;
			projectile.y += p.vel.y * deltaTime;
		}
	}

	// Clear list of projectiles
	void Clear() {
		projectiles.clear();
	}

	// Add a projectile
	void Add(SDL_FPoint point) {
		projectiles.push_back(point);
	}

	// Check for Collisions with the Player
	void CheckCollisionsAndBounds(Player& player, SDL_Point& window_size) {
		auto length = projectiles.size();

		for (int i = 0; i < length; i++) {
			auto proj = projectiles[i];
			bool is_dead = false;
			if (check_collision(player.pos, proj, player.radius, p.radius * p.scale)) {
				player.is_dead = true;
				is_dead = true;
			}
			// Remove the projectile if dead
			if (is_dead || proj.y > 1000 || proj.x < -100 || proj.x > window_size.x + 100) {
				projectiles[i] = projectiles[--length];
				i = SDL_max(i - 1, 0);
			}
		}

		projectiles.resize(length);
	}

	// Check for Collisions with an enemy 
	void CheckCollisionsAndBounds(Enemy& enemy, SDL_Point& window_size) {
		auto length = projectiles.size();

		for (int i = 0; i < length; i++) {
			auto proj = projectiles[i];
			bool is_dead = false;
			if (check_collision(enemy.pos, proj, enemy.radius, p.radius * p.scale)) {
				p.Damage(enemy);
				is_dead = true;
			}
			// Remove the projectile if dead
			if (is_dead || proj.y > 1000 || proj.x < -100 || proj.x > window_size.x + 100) {
				projectiles[i] = projectiles[--length];
				i = SDL_max(i - 1, 0);
			}
		}

		projectiles.resize(length);
	}

	// Render Projectiles on the screen
	void DrawProjectiles(SDL_Renderer* renderer, TextureManager& texture) {
		SDL_Rect projectile_sprite = texture.sprite_storage[p.sprite].bounds;

		SDL_SetTextureColorMod(texture.texture, p.color.r, p.color.g, p.color.b);
		for (auto& proj : projectiles) {
			SDL_FRect proj_rect = { proj.x, proj.y, (float)projectile_sprite.w * p.scale, (float)projectile_sprite.h * p.scale };

			SDL_RenderCopyF(renderer, texture.texture, &projectile_sprite, &proj_rect);
		}
		SDL_SetTextureColorMod(texture.texture, 255, 255, 255);
	}
};