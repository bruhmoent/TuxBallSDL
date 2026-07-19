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

#include <SDL3_mixer/SDL_mixer.h>
#include <fstream>

#include "game.hpp"
#include "ECS/Components.h"
#include "ParticleExample.h"
#include "ParticleSystem.h"
#include "collision.hpp"
#include "config.hpp"
#include "input_handler.hpp"
#include "rectangle.hpp"
#include "texture_manager.hpp"
#include "tilemap_object.hpp"
#include "vector_math.hpp"

Map* map;
SDL_Renderer* Game::renderer = nullptr;
Manager manager;
SDL_Event Game::event;
std::vector<ColliderComponent*> Game::colliders;
auto& player(manager.addEntity());
SDL_Rect Game::camera = { 0, 0, 800, 640 };
int images[25][25] = {};
SDL_Texture* Game::mapTexture = nullptr;

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
std::vector<Rectangle*> blocks = {};

ParticleExample* para = new ParticleExample();

void
Game::init(const char* title,
           int xpos,
           int ypos,
           int width,
           int height,
           bool fullscreen)
{
  int flags = 0;
  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    window = SDL_CreateWindow(title, width, height, flags);
    renderer = SDL_CreateRenderer(window, nullptr);

    if (renderer) {
      SDL_SetRenderDrawColor(renderer, 125, 125, 255, 255);
      para->setRenderer(renderer);
      para->setPosition(400, 0);
      para->setStyle(ParticleExample::SNOW);
      para->addParticles(20);
    }
    isRunning = true;

    std::string mapfile_path =
      (std::filesystem::path(Config::get_project_root()) / "assets" / "images" /
       "tileTs.bmp")
        .string();
    mapTexture = TextureManager::LoadTexture(mapfile_path.c_str());
    if (!mapTexture) {
      std::cout << "CRITICAL: Failed to load map texture!" << std::endl;
    }

    std::string map_path = (std::filesystem::path(Config::get_project_root()) /
                            "assets" / "levels" / "map3.txt")
                             .string();
    map = new Map();
    blocks = Map::loadMap(map_path, 25, 20);

    player.addComponent<TransformComponent>(2);
    std::string player_sprite_path =
      Config::get_project_root() + "/assets/images/tuxball.png";
    player.addComponent<SpriteComponent>(player_sprite_path.c_str());
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
    player.addGroup(groupMap);

    if (!MIX_Init()) {
      std::cout << "MIX_Init Error: " << SDL_GetError() << std::endl;
    }

    MIX_Mixer* g_mixer =
      MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!g_mixer) {
      std::cout << "MIX_CreateMixerDevice Error: " << SDL_GetError()
                << std::endl;
    } else {
      std::string path =
        Config::get_project_root() + "/assets/audio/chipdisko.ogg";

      MIX_Audio* music = MIX_LoadAudio(g_mixer, path.c_str(), false);
      if (!music) {
        std::cout << "Music Error: " << SDL_GetError() << std::endl;
      } else {
        MIX_Track* musicTrack = MIX_CreateTrack(g_mixer);
        MIX_SetTrackAudio(musicTrack, music);

        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);

        MIX_PlayTrack(musicTrack, props);
        SDL_DestroyProperties(props);
      }
    }
  } else {
    isRunning = false;
  }
}

void
Game::handleEvents()
{
  SDL_PollEvent(&event);
  switch (event.type) {
    case SDL_EVENT_QUIT:
      isRunning = false;
      break;
  }
};

bool
Game::HasCollision(int xpos, int ypos)
{
  SDL_Rect* p = new SDL_Rect;
  p->x = xpos;
  p->y = ypos;
  p->w = 64;
  p->h = 64;

  for (auto i : blocks) {
    SDL_Rect* b = new SDL_Rect;
    b->x = i->x;
    b->y = i->y;
    b->w = i->w;
    b->h = i->h;
    bool colision = SDL_HasRectIntersection(p, b);
    if (colision) {
      return true;
    }
  }
  return false;
}

bool
Game::HasCollisionP(int xpos, int ypos)
{

  SDL_Rect* p = new SDL_Rect;
  p->x = xpos;
  p->y = ypos;
  p->w = 64;
  p->h = 64;

  for (auto i : blocks) {
    SDL_Rect* b = new SDL_Rect;
    b->x = i->x;
    b->y = i->y - 3;
    b->w = i->w;
    b->h = i->h;
    bool colision = SDL_HasRectIntersection(p, b);
    if (colision) {
      return true;
    }
  }
  return false;
}

bool
Game::cCol()
{
  return collision;
}

bool
Game::cColP()
{
  return collisionP;
}
void
Game::uCol()
{
  collision = false;
}

void
Game::update()
{
  manager.refresh();
  manager.update();
  Vector2D playerPos = player.getComponent<TransformComponent>().position;
  if (HasCollision(player.getComponent<TransformComponent>().position.x,
                   player.getComponent<TransformComponent>().position.y)) {
    player.getComponent<TransformComponent>().position = playerPos;
    collision = true;
  } else {
    collision = false;
  }
  if (HasCollisionP(player.getComponent<TransformComponent>().position.x,
                    player.getComponent<TransformComponent>().position.y)) {
    player.getComponent<TransformComponent>().position = playerPos;
    collisionP = true;
  } else {
    collisionP = false;
  }

  // Camera adjustment feature (mouse movement on button pressed)
  float mouseX, mouseY;
  if (SDL_GetMouseState(&mouseX, &mouseY) != 0) {
    SDL_SetWindowRelativeMouseMode(window, true);
    camera.x =
      int(player.getComponent<TransformComponent>().position.x - 400.0f) +
      (int)mouseX - (camera.w / 2);
    camera.y =
      int(player.getComponent<TransformComponent>().position.y - 320.0f) +
      (int)mouseY - (camera.h / 2);
  } else {
    SDL_SetWindowRelativeMouseMode(window, false);
    camera.x =
      int(player.getComponent<TransformComponent>().position.x - 400.0f);
    camera.y =
      int(player.getComponent<TransformComponent>().position.y - 320.0f);
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

void
Game::render()
{
  SDL_RenderClear(renderer);
  para->draw();
  SDL_SetRenderDrawColor(renderer, 124, 184, 217, 255);
  for (auto& t : tiles) {
    t->draw();
  }
  for (auto& p : players) {
    p->draw();
  }
  for (auto& e : enemies) {
    e->draw();
  }
  // Draw the custom cursor

  std::string cursor_path =
    Config::get_project_root() + "/assets/images/cursor.png";

  SDL_Texture* texture = IMG_LoadTexture(renderer, cursor_path.c_str());
  // SDL_Texture* textureSnap = IMG_LoadTexture(renderer, "gridsnap.png");
  // SDL_Rect rectSnap;
  SDL_FRect rect;
  rect.w = 64.0f;
  rect.h = 64.0f;
  float mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  rect.x = mouseX;
  rect.y = mouseY;
  SDL_RenderTexture(renderer, texture, NULL, &rect);
  // SDL_RenderCopy(renderer, textureSnap, NULL, &rectSnap);
  SDL_RenderPresent(renderer);
};

void
Game::clean()
{
  SDL_DestroyTexture(mapTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  delete para;
};

void
Game::AddTile(int srcX, int srcY, int xpos, int ypos, int x, int y, int kind)
{
  auto& tile(manager.addEntity());

  tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapTexture);

  tile.addGroup(groupMap);
  images[y][x] = kind;
}

// TO DO: Implement a RemoveTile function
void
Game::AddBlock(Rectangle* rectangle)
{
  blocks.push_back(rectangle);
}

Point*
Game::GetPlayerPosition()
{
  Vector2D vector = player.getComponent<TransformComponent>().position;

  Point* point = new Point();
  point->x = vector.x;
  point->y = vector.y;

  return point;
}

Point*
Game::GetCameraPosition()
{
  Point* point = new Point();
  point->x = camera.x;
  point->y = camera.y;

  return point;
}

void
Game::backToPriorPosition(float x, float y)
{
  Vector2D vector = player.getComponent<TransformComponent>().position;
  vector.x = x;
  vector.y = y;
}
