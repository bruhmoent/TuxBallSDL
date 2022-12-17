#include "game.hpp"
#include "TextureManager.h"
#include "tilemapObj.h"
#include "ECS/Components.h"
#include "vectorHandler.h"
#include "Collision.h"
#include <fstream>
#include "inputHandler.h"
#include "ParticleExample.h"
#include "ParticleSystem.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;

SDL_Event Game::event;
std::vector<ColliderComponent*> Game::colliders;
auto& player(manager.addEntity());
SDL_Rect Game::camera = { 0,0,800,640 };
int images[25][25] = {};
const char* mapfile = "const_assets/tileTs.png";
bool collision = false;
bool collisionP = false;
  
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
std::vector<Rectagle*> blocks = {};


Game::Game(){}
Game::~Game(){}
ParticleExample* para = new ParticleExample();
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 125, 125, 255, 255);

			para->setRenderer(renderer);
			para->setPosition(400, 0); 
			
			para->setStyle(ParticleExample::SNOW);
			para->addParticles(20);
		}
		isRunning = true;


		map = new Map();

		blocks = Map::loadMap("const_assets/map3.txt", 25, 20);
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
bool Game::HasCollision(int xpos, int ypos) {

	SDL_Rect* p = new SDL_Rect;
	p->x = xpos;
	p->y = ypos;
	p->w = 64;
	p->h = 64;

	for (auto i : blocks)
	{
		SDL_Rect* b = new SDL_Rect;
		b->x = i->x;
		b->y = i->y;
		b->w = i->w;
		b->h = i->h;
		bool colision = SDL_HasIntersection(p, b);
		if (colision)
		{
			return true;
		}
	}
	return false;

}

bool Game::HasCollisionP(int xpos, int ypos) {

	SDL_Rect* p = new SDL_Rect;
	p->x = xpos;
	p->y = ypos;
	p->w = 64;
	p->h = 64;

	for (auto i : blocks)
	{
		SDL_Rect* b = new SDL_Rect;
		b->x = i->x;
		b->y = i->y - 3;
		b->w = i->w;
		b->h = i->h;
		bool colision = SDL_HasIntersection(p, b);
		if (colision)
		{
			return true;
		}
	}
	return false;
}

bool Game::cCol() {
	return collision;
}

bool Game::cColP() {
	return collisionP;
}
void Game::uCol() {
	collision = false;
}
void Game::update()
{
	manager.refresh();
	manager.update();
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	if (HasCollision(player.getComponent<TransformComponent>().position.x, player.getComponent<TransformComponent>().position.y))
	{
		player.getComponent<TransformComponent>().position = playerPos;
		collision = true;
	}
	else {
		collision = false;
	}
	if (HasCollisionP(player.getComponent<TransformComponent>().position.x, player.getComponent<TransformComponent>().position.y))
	{
		player.getComponent<TransformComponent>().position = playerPos;
		collisionP = true;
	}
	else {
		collisionP = false;
	}
	camera.x = int(player.getComponent<TransformComponent>().position.x - 400.0f);
	camera.y = int(player.getComponent<TransformComponent>().position.y - 320.0f);
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

};

void Game::render()
{
	SDL_RenderClear(renderer);

	para->draw();
	SDL_SetRenderDrawColor(renderer, 124, 184, 217, 255);
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
	delete para;
};

void Game::AddTile(int srcX, int srcY, int xpos, int ypos, int x, int y, int kind)
{
	auto& tile(manager.addEntity());

	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);

	tile.addGroup(groupMap);
	auto& tile2(tile.getComponent<TileComponent>());

	images[y][x] = kind;
	int h = 9;
}


Point* Game::GetPlayerPosition() {
	Vector2D vector = player.getComponent<TransformComponent>().position;

	Point* point = new Point();
	point->x = vector.x;
	point->y = vector.y;

	return point;
}

void Game::backToPriorPosition(float x, float y) {
	Vector2D vector = player.getComponent<TransformComponent>().position;

	vector.x = x;
	vector.y = y;
}


