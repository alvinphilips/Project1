#pragma once

#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <list>

#include "Object.h"

class ObjectManager
{
	std::list<Object*> objects;
public:
	void Initialize()
	{
		for (const auto& object : objects)
		{
			object->Initialize();
		}
	}
	void Update(const float deltaTime)
	{
		for (const auto& object : objects)
		{
			object->Update(deltaTime);
		}
		objects.remove_if([&](const Object* obj) {return obj->is_dead; });
	}
	void AddObject(Object* object)
	{
		object->Initialize();
		objects.push_back(object);
	}
	void Destroy()
	{
		for (const auto& object: objects)
		{
			object->Destroy();
			delete object;
		}
		objects.clear();
	}
};

#endif