#pragma once
#define Game_hpp
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Point.h"
class ColliderComponent;
class Game {
public:
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static void AddTile(int srcX, int srcY, int xpos, int ypos, int x, int y, int kind);
	static bool HasCollision(int xpos, int ypos);
	static bool HasCollisionP(int xpos, int ypos);
	static SDL_Rect camera;
	static bool cCol();
	static bool cColP();
	static void uCol();
	static void backToPriorPosition(float x, float y);
	static Point* GetPlayerPosition();
	bool running() {
		return isRunning;
	};

private:
	bool isRunning;
	SDL_Window* window;
};	