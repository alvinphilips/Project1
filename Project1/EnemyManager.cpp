#include "EnemyManager.h"

ProjectileManager bullets;

void EnemyManager::Initialize() {
	bullets.Clear();
}
void EnemyManager::SetWindowSizeAndPlayer(SDL_Point* _window_size, Player* _player) {
	window_size = _window_size;
	player = _player;
}
void EnemyManager::AddEnemy(const Enemy& prefab) {
	Enemy e = prefab;
	ResetEnemy(e);
	enemies.push_back(e);
}
unsigned int EnemyManager::UpdateAndGetScore(const float deltaTime, ProjectileManager& player_bullets) {
	unsigned int score = 0;
	for (auto& enemy : enemies) {
		enemy.Update(deltaTime);

		if (enemy.pos.y > window_size->y || enemy.pos.x < -100 || enemy.pos.x > window_size->x + 100) {
			ResetEnemy(enemy);
		}

		// Calculate collisions
		if (!can_collide) {
			continue;
		}

		if (check_collision(player->pos, enemy.pos, player->radius, enemy.radius * enemy.scale)) {
			enemy.is_dead = true;
			player->is_dead = true;
		}
		player_bullets.CheckCollisionsAndBounds(enemy, *window_size);
		// Reset dead Enemies
		if (enemy.is_dead) {
			score += enemy.points;
			ResetEnemy(enemy);
			enemy.is_dead = false;
		} else if (enemy.shot_ready) {
			// The dead can't shoot
			SDL_FPoint bullet_pos = { enemy.pos.x + enemy.radius, enemy.pos.y + enemy.radius };
			bullets.Add(bullet_pos);
			enemy.shot_ready = false;
		}
	}
	bullets.Update(deltaTime);
	bullets.CheckCollisionsAndBounds(*player, *window_size);

	return score;
}
void EnemyManager::DrawEnemies(SDL_Renderer* renderer, TextureManager& texture) {
	for (auto& enemy : enemies) {
		SDL_Rect enemy_sprite = texture.sprite_storage[enemy.sprite].bounds;

		// Scale enemy by half
		SDL_FRect enemy_rect = { enemy.pos.x, enemy.pos.y, (float)enemy_sprite.w * enemy.scale, (float)enemy_sprite.h * enemy.scale };

		SDL_SetTextureColorMod(texture.texture, enemy.color.r, enemy.color.g, enemy.color.b);
		// SDL_RenderCopyF(renderer, texture.texture, &enemy_sprite, &enemy_rect);
		SDL_RenderCopyExF(renderer, texture.texture, &enemy_sprite, &enemy_rect, 0, nullptr, SDL_FLIP_VERTICAL);
		SDL_SetTextureColorMod(texture.texture, 255, 255, 255);
	}

	bullets.DrawProjectiles(renderer, texture);
}
void EnemyManager::Destroy() {
	player = nullptr;
	window_size = nullptr;
	for (auto& enemy : enemies) {
		enemy.Destroy();
	}
	enemies.clear();
}