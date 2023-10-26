#include "Engine.h"

void Engine::ResetGame() {
	player = prefab.player;
	// Center the Player to the screen
	player.pos = SDL_FPoint{ (float)window_size.x / 2.0f - (float)player.radius, (float)window_size.y / 3.0f };

	// Initialize both EnemyManagers
	enemies.Initialize();
	decorations.Initialize();
	enemies.ResetAllEnemies();
	decorations.ResetAllEnemies();

	// Lets be nice, and destroy all the Projectiles currently spawned, if any
	player_projectiles.Clear();
	enemies.bullets.Clear();

	high_score = SDL_max(high_score, score);
	// Set score to 0
	score = 0;
}

void Engine::Initialize() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_size.x, window_size.y, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	text.Initialize();
	texture.Initialize(renderer);

	SDL_SetRenderDrawColor(renderer, window_clear_color.r, window_clear_color.g, window_clear_color.b, window_clear_color.a);

	quit = false;

	enemies.AddEnemy(prefab.enemies["asteroid_large"]);
	enemies.AddEnemy(prefab.enemies["asteroid_large"]);
	enemies.AddEnemy(prefab.enemies["asteroid_small"]);
	enemies.AddEnemy(prefab.enemies["asteroid_small"]);
	enemies.AddEnemy(prefab.enemies["enemy_light"]);
	enemies.AddEnemy(prefab.enemies["enemy_light"]);
	enemies.AddEnemy(prefab.enemies["enemy_light"]);
	enemies.AddEnemy(prefab.enemies["enemy_heavy"]);
	enemies.AddEnemy(prefab.enemies["enemy_heavy"]);
	enemies.SetWindowSizeAndPlayer(&window_size, &player);
	enemies.bullets.p = prefab.enemy_projectile;

	// Add 30 small and medium stars, each
	for (int i = 0; i < 30; i++) {
		decorations.AddEnemy(prefab.decorations["star_small"]);
		decorations.AddEnemy(prefab.decorations["star_medium"]);
	}
	decorations.SetWindowSizeAndPlayer(&window_size, &player);

	// Disable collisions on decorations
	decorations.can_collide = false;

	player_projectiles.p = prefab.player_projectile;

	ResetGame();
}
void Engine::HandleKeyboardInput(SDL_Event& event) {
	is_shooting = false;
	switch (event.key.keysym.sym) {
	case SDLK_UP:
	case SDLK_w:
		player.vel.y -= player.movement_factor;
		break;
	case SDLK_DOWN:
	case SDLK_s:
		player.vel.y += player.movement_factor;
		break;
	case SDLK_LEFT:
	case SDLK_a:
		player.vel.x -= player.movement_factor;
		break;
	case SDLK_RIGHT:
	case SDLK_d:
		player.vel.x += player.movement_factor;
		break;
	case SDLK_SPACE:
		is_shooting = true;
		break;
	case SDLK_ESCAPE:
		quit = true;
		break;
	case SDLK_l:
		LoadGame();
		break;
	case SDLK_BACKQUOTE:
		SaveGame();
		break;
	default:
		SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Key Pressed: %ui", event.key.keysym.sym);
	}

	// Clamp player velocity
	player.vel.x = SDL_clamp(player.vel.x, -player.speed, player.speed);
	player.vel.y = SDL_clamp(player.vel.y, -player.speed, player.speed);
}
void Engine::HandleInput() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
				quit = true;
			}
			break;
		case SDL_KEYDOWN:
			HandleKeyboardInput(event);
			break;
		default:
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Unhandled event: %ui", event.type);
		}
	}
}

void Engine::Update(const float deltaTime) {
	SDL_RenderClear(renderer);
	text.DrawText(renderer, "LIVES: " + std::to_string(player.lives));
	text.DrawText(renderer, "SCORE: " + std::to_string(score), { 0, 30 });
	text.DrawText(renderer, "HIGHSCORE: " + std::to_string(high_score), { 0, 60 });

#ifdef _DEBUG_PLAYER
	text.DrawText(renderer, "pos: " + std::to_string(player.pos.x) + ", " + std::to_string(player.pos.y), { 0, 90 });
	text.DrawText(renderer, "vel: " + std::to_string(player.vel.x) + ", " + std::to_string(player.vel.y), { 0, 120 });
	text.DrawText(renderer, "speed: " + std::to_string(player.speed), { 0, 150 });
#endif

	player.Update(deltaTime);

	if (is_shooting && player.can_fire) {
		player_projectiles.Add(player.pos + SDL_FPoint{ (float)player.radius, (float)player.radius * -1.0f });
		player.can_fire = false;
	}

	player_projectiles.Update(deltaTime);

	SDL_Rect player_size = texture.sprite_storage[player.sprite].bounds;
	player.pos.x = SDL_clamp(player.pos.x, 20, window_size.x - (20 + player_size.w));
	player.pos.y = SDL_clamp(player.pos.y, 20, window_size.y - (20 + player_size.h));

	score += enemies.UpdateAndGetScore(deltaTime, player_projectiles);

	decorations.UpdateAndGetScore(deltaTime, player_projectiles);

	// Check if player is dead
	if (player.is_dead) {
		player.lives--;
		player.is_dead = false;
	}

	// Restart game if we have no more lives
	if (player.lives == 0) {
		ResetGame();
	}

	SDL_FRect dst = { player.pos.x, player.pos.y, (float)player_size.w, (float)player_size.h };

	decorations.DrawEnemies(renderer, texture);
	enemies.DrawEnemies(renderer, texture);
	player_projectiles.DrawProjectiles(renderer, texture);

	// Render Player
	SDL_SetTextureColorMod(texture.texture, player.color.r, player.color.g, player.color.b);
	SDL_RenderCopyF(renderer, texture.texture, &player_size, &dst);
	SDL_SetTextureColorMod(texture.texture, 255, 255, 255);

	SDL_RenderPresent(renderer);
}

void Engine::MainLoop() {
	Uint64 current_time = SDL_GetPerformanceCounter();
	Uint64 previous_time = 0;
	float deltaTime = 0;

	while (!quit) {
		// Calculate delta time
		// See: https://gamedev.stackexchange.com/questions/110825/how-to-calculate-delta-time-with-sdl
		previous_time = current_time;
		current_time = SDL_GetPerformanceCounter();
		deltaTime = (float)((current_time - previous_time) * 1000 / (double)SDL_GetPerformanceFrequency());

		HandleInput();
		Update(deltaTime);

		// Dampen velocity, used to give a sense of inertia to the player
		player.vel.x *= player.velocity_dampener;
		player.vel.y *= player.velocity_dampener;
	}
}

// Clean up after ourselves, and reset all pointers
void Engine::Destroy() {
	prefab.Destroy();
	text.Destroy();
	texture.Destroy();
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

template <>
void load_from_json(Engine& engine, const json::JSON& node) {
	if (node.hasKey("title")) {
		load_from_json(engine.window_title, node.at("title"));
	}

	if (node.hasKey("window")) {
		const json::JSON& window = node.at("window");
		if (window.hasKey("width")) {
			load_from_json(engine.window_size.x, node.at("window").at("width"));
		}
		if (window.hasKey("height")) {
			load_from_json(engine.window_size.y, node.at("window").at("height"));
		}
		if (window.hasKey("clear_color")) {
			load_from_json(engine.window_clear_color, node.at("window").at("clear_color"));
		}
	}

	if (node.hasKey("config")) {
		const json::JSON& config = node.at("config");

		if (config.hasKey("font")) {
			const json::JSON font_file = give_me_json(config.at("font").ToString().c_str());
			load_from_json(engine.text, font_file);
		}

		if (config.hasKey("texture")) {
			const json::JSON texture_file = give_me_json(config.at("texture").ToString().c_str());
			load_from_json(engine.texture, texture_file);
		}

		// Load Prefabs, NEEDS to be loaded after textures
		if (config.hasKey("prefab")) {
			const json::JSON prefab_file = give_me_json(config.at("prefab").ToString().c_str());
			// Set PrefabManager's texture field
			engine.prefab.SetTextureManager(&engine.texture);
			load_from_json(engine.prefab, prefab_file);
		}

		if (config.hasKey("save")) {
			load_from_json(engine.save_file, config.at("save"));
		}
	}
}
