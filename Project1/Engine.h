#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "TextManager.h"
#include "TextureManager.h"
#include "PrefabManager.h"
#include "Enemy.h"
#include "Player.h"

#include <fstream>

inline json::JSON give_me_json(const char* filename) {
	std::ifstream input_stream(filename);
	std::string str((std::istreambuf_iterator<char>(input_stream)), std::istreambuf_iterator<char>());
	return json::JSON::Load(str);
}

class Engine
{
public:
	TextManager text;
	TextureManager texture;
	PrefabManager prefab;
	SDL_Point window_size;
	SDL_Color window_clear_color;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::string window_title;
	long score;
	bool quit;
	bool is_shooting;

	Player player;

	void Initialize()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_size.x, window_size.y, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		text.Initialize();
		texture.Initialize(renderer);

		SDL_SetRenderDrawColor(renderer, window_clear_color.r, window_clear_color.g, window_clear_color.b, window_clear_color.a);

		score = 0;
		quit = false;
		
		player = prefab.player;
		player.pos = SDL_FPoint{ (float) window_size.x / 2.0f - (float) player.radius, (float) window_size.y / 3.0f };
	}
	void HandleKeyboardInput(SDL_Event& event)
	{
		is_shooting = false;
		switch (event.key.keysym.sym)
		{
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
		default:
			SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Key Pressed: %ui", event.key.keysym.sym);
		}

		// Clamp player velocity
		player.vel.x = SDL_clamp(player.vel.x, -player.speed, player.speed);
		player.vel.y = SDL_clamp(player.vel.y, -player.speed, player.speed);
	}
	void HandleInput()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
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

	void Update(const float deltaTime)
	{
		SDL_RenderClear(renderer);
		text.DrawText(renderer, "SCORE: " + std::to_string(score));

#define _DEBUG_PLAYER
#ifdef _DEBUG_PLAYER
		text.DrawText(renderer, "pos: " + std::to_string(player.pos.x) + ", " + std::to_string(player.pos.y), {0, 30});
		text.DrawText(renderer, "vel: " + std::to_string(player.vel.x) + ", " + std::to_string(player.vel.y), {0, 60});
		text.DrawText(renderer, "speed: " + std::to_string(player.speed), {0, 90});
#endif

		player.Update(deltaTime);

		if (is_shooting && player.can_fire)
		{
			// TODO: Shoot!
		}
		SDL_Rect player_size = texture.sprite_storage[player.sprite].bounds;
		player.pos.x = SDL_clamp(player.pos.x, 20, window_size.x - (20 + player_size.w));
		player.pos.y = SDL_clamp(player.pos.y, 20, window_size.y - (20 + player_size.h));

		SDL_FRect dst = { player.pos.x, player.pos.y, (float) player_size.w, (float) player_size.h };

		SDL_RenderCopyF(renderer, texture.texture, &player_size, &dst);

		SDL_RenderPresent(renderer);
	}

	void MainLoop()
	{
		Uint64 current_time = SDL_GetPerformanceCounter();
		Uint64 previous_time = 0;
		float deltaTime = 0;

		while (!quit)
		{
			// Calculate delta time
			// See: https://gamedev.stackexchange.com/questions/110825/how-to-calculate-delta-time-with-sdl
			previous_time = current_time;
			current_time = SDL_GetPerformanceCounter();
			deltaTime = (float) ((current_time - previous_time)*1000 / (double) SDL_GetPerformanceFrequency());

			HandleInput();
			Update(deltaTime);

			// Dampen velocity, used to give a sense of inertia to the player
			player.vel.x *= player.velocity_dampener;
			player.vel.y *= player.velocity_dampener;
		}
	}

	// Clean up after ourselves, and reset all pointers
	void Destroy()
	{
		prefab.Destroy();
		text.Destroy();
		texture.Destroy();
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		SDL_Quit();
	}
};

template <>
void load_from_json(Engine& value, const json::JSON& node);

#endif
