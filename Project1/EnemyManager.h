#pragma once

#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <list>

#include "Enemy.h"

class EnemyManager
{
	std::list<Enemy*> enemies;
public:
	void Initialize()
	{
		for (auto& enemy : enemies)
		{
			enemy->Initialize();
		}
	}
	void Update(const float deltaTime)
	{
		for (auto& enemy : enemies)
		{
			enemy->Update(deltaTime);
		}
	}
	void Destroy()
	{
		for (auto& enemy: enemies)
		{
			enemy->Destroy();
			delete enemy;
		}
		enemies.clear();
	}
};

#endif