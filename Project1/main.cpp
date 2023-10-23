#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "Text.h"

int main(int argc, char** argv)
{
	TextManager text;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("window title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	text.font_file = "Roboto.ttf";
	text.size = 24;
	text.Initialize();

	SDL_SetRenderDrawColor(renderer, 23, 66, 234, 255);

	SDL_Surface* surface = IMG_Load("texture.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	long score = 200;

	bool quit = false;
	SDL_Event event;

	while (!quit)
	{
		score++;

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
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				default:
					SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Key Pressed: %ui", event.key.keysym.sym);
				}
				break;
			default:
				SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Unhandled event: %ui", event.type);
			}
		}

		SDL_RenderClear(renderer);
		text.DrawText(renderer, "SCORE: " + std::to_string(score));
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}