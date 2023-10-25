#include "Engine.h"


template <>
void load_from_json(Engine& engine, const json::JSON& node)
{
	if (node.hasKey("title"))
	{
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

		if (config.hasKey("font"))
		{
			const json::JSON font_file = give_me_json(config.at("font").ToString().c_str());
			load_from_json(engine.text, font_file);
		}

		if (config.hasKey("texture"))
		{
			const json::JSON texture_file = give_me_json(config.at("texture").ToString().c_str());
			load_from_json(engine.texture, texture_file);
		}

		// Load Prefabs, NEEDS to be loaded after textures
		if (config.hasKey("prefab"))
		{
			const json::JSON prefab_file = give_me_json(config.at("prefab").ToString().c_str());
			// Set PrefabManager's texture field
			engine.prefab.SetTextureManager(&engine.texture);
			load_from_json(engine.prefab, prefab_file);
		}

	}
}
