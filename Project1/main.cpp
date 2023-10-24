#include <SDL/SDL.h>

#include "Engine.h"

int main(int argc, char** argv)
{
	const json::JSON settings_file = give_me_json("settings.json");
	Engine engine;
	load_from_json(engine, settings_file);
	engine.Initialize();
	engine.MainLoop();
	engine.Destroy();
	return 0;
}