#pragma once

#ifndef ENGINE_H
#define ENGINE_H

#include "TextManager.h"
#include "TextureManager.h"
#include "PrefabManager.h"
#include "Enemy.h"
#include "Player.h"
#include "EnemyManager.h"

#include <fstream>

inline json::JSON give_me_json(const char* filename) {
	std::ifstream input_stream(filename);
	std::string str((std::istreambuf_iterator<char>(input_stream)), std::istreambuf_iterator<char>());
	return json::JSON::Load(str);
}

class Engine {
public:
	TextManager text;
	TextureManager texture;
	PrefabManager prefab;
	EnemyManager enemies;
	EnemyManager decorations;

	ProjectileManager player_projectiles;

	SDL_Point window_size;
	SDL_Color window_clear_color;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	std::string window_title;
	long score;
	long high_score;
	bool quit;
	bool is_shooting;

	std::string save_file;

	Player player;

	void ResetGame();
	void LoadGame() {
		json::JSON node = give_me_json(save_file.c_str());
		load_from_json(enemies, node.at("enemies"));
		load_from_json(player, node.at("player"));

		load_from_json(decorations, node.at("decorations"));
		score = node.at("score").ToInt();
		// Initialize both EnemyManagers
		enemies.Initialize();
		decorations.Initialize();

		// Lets be nice, and destroy all the Projectiles currently spawned, if any
		player_projectiles.Clear();

		high_score = SDL_max(high_score, score);
	}
	void SaveGame() {
		json::JSON node;
		node["enemies"] = save_to_json(enemies);
		node["player"] = save_to_json(player);
		node["decorations"] = save_to_json(decorations);
		node["score"] = score;

		std::ofstream ostrm(save_file.c_str());
		ostrm << node.dump() << std::endl;
		ostrm.close();
	}

	void Initialize();
	void HandleKeyboardInput(SDL_Event& event);
	void HandleInput();

	void Update(const float);
	void MainLoop();
	// Clean up after ourselves, and reset all pointers
	void Destroy();
};

template <>
void load_from_json(Engine&, const json::JSON&);

#endif
