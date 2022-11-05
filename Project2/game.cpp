
#include "game.hpp"
#include "TextureManager.h"
#include "tilemapObj.h"
#include "ECS/Components.h"
#include "vectorHandler.h"
#include "Collision.h"
Map* map;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;

SDL_Event Game::event;
std::vector<ColliderComponent*> Game::colliders;
auto& player(manager.addEntity());
SDL_Rect Game::camera = { 0,0,800,640 };
int images[25][25] = {};
const char* mapfile = "const_assets/tiles.png";
enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

Game::Game()
{

}
Game::~Game()
{

}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Sub-game initalized!" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "Window created." << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created." << std::endl;
		}
		isRunning = true;


		map = new Map();

		//Usage of ECS.h
		Map::loadMap("const_assets/map3.txt", 25, 20);
		player.addComponent<TransformComponent>(2);
		player.addComponent<SpriteComponent>("sprites/tuxball.png");
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>("player");
		player.addGroup(groupPlayers);


		player.addGroup(groupMap);
	}
	else
	{
		isRunning = false;
	}
};

void Game::handleEvents()
{

	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	}

};
void Game::update()
{

	manager.refresh();
	manager.update();

	camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

	//std::cout << player.getComponent<TransformComponent>().position.x;

};

void Game::render()
{
	SDL_RenderClear(renderer);
	//render-body
	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}
	SDL_RenderPresent(renderer);
};
void Game::clean()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
};

void Game::AddTile(int srcX, int srcY, int xpos, int ypos, int x, int y, int kind)
{
	auto& tile(manager.addEntity());

	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);

	tile.addGroup(groupMap);
	auto& tile2(tile.getComponent<TileComponent>());

	images[y][x] = kind;
	//tile2.position
	int h = 9;
}

bool Game::HasCollision(int xpos, int ypos) {

	int x = xpos / 64 +1;
	int y = ypos / 64;

	//std::cout << "Zdarzenie " << xpos << ":" << ypos << " (" << x << ":" << y << ") -> " << images[y][x] << "\n";

	return images[y][x] == 0;
}