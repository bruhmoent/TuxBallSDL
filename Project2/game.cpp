//  TuxBall
//  Copyright (C) 2022 bruhmoent
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include "SDL_mixer.h"
#include "Rectagle.h"

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

		//Assign the map
		map = new Map();
		blocks = Map::loadMap("const_assets/map3.txt", 25, 20);

		//Group entities (layering)
		player.addComponent<TransformComponent>(2);
		player.addComponent<SpriteComponent>("sprites/tuxball.png");
		player.addComponent<KeyboardController>();
		player.addComponent<ColliderComponent>("player");
		player.addGroup(groupPlayers);
		player.addGroup(groupMap);
		
		//Music Init
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
		Mix_Music* music = Mix_LoadMUS("audio/chipdisko.ogg");
		if (!music) {
			std::cout << "Music Error:" << Mix_GetError() << std::endl;
		}
		Mix_PlayMusic(music, -1);
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
		if (colision){ return true; }
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

	//Camera adjustment feature (mouse movement on button pressed)
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	if (SDL_GetMouseState(&mouseX, &mouseY) && SDL_MOUSEMOTION) {
		SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		camera.x = int(player.getComponent<TransformComponent>().position.x - 400.0f) + mouseX - (camera.w / 2);
		camera.y = int(player.getComponent<TransformComponent>().position.y - 320.0f) + mouseY - (camera.h / 2);
	}
	else {
		camera.x = int(player.getComponent<TransformComponent>().position.x - 400.0f);
		camera.y = int(player.getComponent<TransformComponent>().position.y - 320.0f);
	}
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
	//Draw the custom cursor
	SDL_Texture* texture = IMG_LoadTexture(renderer, "cursor.png");
	//SDL_Texture* textureSnap = IMG_LoadTexture(renderer, "gridsnap.png");
	//SDL_Rect rectSnap;
	SDL_Rect rect;
	rect.w = 64;
	rect.h = 64;
//	rectSnap.w = 64;
	//rectSnap.h = 64;
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	rect.x = mouseX;
	rect.y = mouseY;
	//rectSnap.x = GetCameraPosition()->x + (mouseX - mouseX % 64);
	//rectSnap.y = GetCameraPosition()->y + (mouseY - mouseY % 64);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	//SDL_RenderCopy(renderer, textureSnap, NULL, &rectSnap);
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
}
//TO DO: Implement a RemoveTile function
void Game::AddBlock(Rectagle* rectangle)
{
	blocks.push_back(rectangle);
}

Point* Game::GetPlayerPosition() {
	Vector2D vector = player.getComponent<TransformComponent>().position;

	Point* point = new Point();
	point->x = vector.x;
	point->y = vector.y;

	return point;
}

Point* Game::GetCameraPosition() {
	Point* point = new Point();
	point->x = camera.x;
	point->y = camera.y;

	return point;
}


void Game::backToPriorPosition(float x, float y) {
	Vector2D vector = player.getComponent<TransformComponent>().position;
	vector.x = x;
	vector.y = y;
}


