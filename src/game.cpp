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

#include "ECS/Components.h"
#include "ParticleExample.h"
#include "ParticleSystem.h"
#include "collision.hpp"
#include "config.hpp"
#include "game.hpp"
#include "input_handler.hpp"
#include "level_data.hpp"
#include "rectangle.hpp"
#include "texture_manager.hpp"
#include "vector_math.hpp"

SINGLETON_CPP(Game);

Game::Game()
  : m_level(std::make_unique<Level>())
  , m_renderer(nullptr)
  , camera({ 0, 0, 800, 640 })
  , m_is_running(true)
  , m_window(nullptr)
{
}

Manager manager;
Entity& player(manager.addEntity());
int images[25][25] = {};

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

ParticleExample* particles_snow = new ParticleExample();

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
    m_window = SDL_CreateWindow(title, width, height, flags);
    m_renderer = SDL_CreateRenderer(m_window, nullptr);

    if (m_renderer) {
      SDL_SetRenderDrawColor(m_renderer, 125, 125, 255, 255);
      particles_snow->setRenderer(m_renderer);
      particles_snow->setPosition(400, 0);
      particles_snow->setStyle(ParticleExample::SNOW);
      particles_snow->addParticles(20);
    }

    m_is_running = true;

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

  m_level->load();
  } else {
    m_is_running = false;
  }
}

void
Game::handle_events()
{
  SDL_PollEvent(&event);
  switch (event.type) {
    case SDL_EVENT_QUIT:
      m_is_running = false;
      break;
  }
};

bool
Game::HasCollision(int xpos, int ypos)
{
  SDL_Rect p = { xpos + 2, ypos + 2, 60, 60 };

  for (const auto& tile : m_level->tiles) {
    SDL_Rect b = { tile->x, tile->y, tile->w, tile->h };
    if (SDL_HasRectIntersection(&p, &b)) {
      return true;
    }
  }
  return false;
}

bool
Game::HasCollisionP(int xpos, int ypos)
{
  SDL_Rect p_feet = { xpos + 10, ypos + 60, 44, 8 };

  for (const auto& tile : m_level->tiles) {
    SDL_Rect b = { tile->x, tile->y, tile->w, tile->h };
    if (SDL_HasRectIntersection(&p_feet, &b)) {
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
Game::update(float dt)
{
  m_delta_time = dt;

  auto& transform = player.getComponent<TransformComponent>();

  Vector2D prev_pos = transform.position;

  manager.refresh();
  manager.update(dt);

  if (HasCollision(transform.position.x, prev_pos.y)) {
    transform.position.x = prev_pos.x;
    transform.velocity.x = 0.0f;
  }

  if (HasCollision(transform.position.x, transform.position.y)) {
    transform.position.y = prev_pos.y;
    transform.velocity.y = 0.0f;
  }

  collision = HasCollision(transform.position.x, transform.position.y);
  collisionP = HasCollisionP(transform.position.x, transform.position.y);

  particles_snow->update(dt);

  // Camera adjustment feature (mouse movement on button pressed)
  float mouseX, mouseY;
  if (SDL_GetMouseState(&mouseX, &mouseY) != 0) {
    SDL_SetWindowRelativeMouseMode(m_window, true);
    camera.x =
      int(player.getComponent<TransformComponent>().position.x - 400.0f) +
      (int)mouseX - (camera.w / 2);
    camera.y =
      int(player.getComponent<TransformComponent>().position.y - 320.0f) +
      (int)mouseY - (camera.h / 2);
  } else {
    SDL_SetWindowRelativeMouseMode(m_window, false);
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
}

void
Game::render()
{
  SDL_RenderClear(m_renderer);
  SDL_SetRenderDrawColor(m_renderer, 124, 184, 217, 255);

  for (auto& t : tiles) {
    t->draw();
  }
  for (auto& p : players) {
    p->draw();
  }
  for (auto& e : enemies) {
    e->draw();
  }

  particles_snow->draw();

  // Draw the custom cursor
  std::string cursor_path =
    Config::get_project_root() + "/assets/images/cursor.png";

  SDL_Texture* texture = IMG_LoadTexture(m_renderer, cursor_path.c_str());
  SDL_FRect rect;
  rect.w = 64.0f;
  rect.h = 64.0f;
  float mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);
  rect.x = mouseX;
  rect.y = mouseY;
  SDL_RenderTexture(m_renderer, texture, NULL, &rect);
  SDL_RenderPresent(m_renderer);
};

void
Game::clean()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
  delete particles_snow;
};

void
Game::add_tile(int srcX, int srcY, int xpos, int ypos, int x, int y, int kind)
{
  auto& tile(manager.addEntity());

  tile.addComponent<TileComponent>(
    srcX, srcY, xpos, ypos, m_level->tileset_texture);

  tile.addGroup(groupMap);
  images[y][x] = kind;
}

// TODO: Implement a RemoveTile function
void
Game::add_block(std::shared_ptr<Rectangle> rectangle)
{
  m_level->tiles.push_back(rectangle);
}

Point
Game::get_player_position()
{
  Vector2D vector = player.getComponent<TransformComponent>().position;

  Point point;
  point.x = vector.x;
  point.y = vector.y;

  return point;
}

Point
Game::get_camera_position()
{
  Point point;
  point.x = camera.x;
  point.y = camera.y;

  return point;
}

void
Game::back_to_prior_position(float x, float y)
{
  Vector2D vector = player.getComponent<TransformComponent>().position;
  vector.x = x;
  vector.y = y;
}
