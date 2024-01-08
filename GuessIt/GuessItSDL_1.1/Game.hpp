#pragma once

#include "SDL.h"
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();

	bool running();

	void render();

	SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
	void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

	void clean();

private:
	bool isRunning = false;
	SDL_Window *window;
	SDL_Renderer *renderer;
};
