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

#ifndef GAME_HPP
#define GAME_HPP

#include "singleton.hpp"

#include <iostream>
#include <stdio.h>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "level.hpp"
#include "point.hpp"
#include "rectangle.hpp"

class ColliderComponent;
class Game
{

SINGLETON(Game)

public:
  Game();
  ~Game() = default;
  void clean();

  void init(const char* title,
            int xpos,
            int ypos,
            int width,
            int height,
            bool fullscreen);

  void handle_events();

  void update(float dt);
  void render();

  void add_tile(int srcX, int srcY, int xpos, int ypos, int x, int y, int kind);
  bool HasCollision(int xpos, int ypos);
  bool HasCollisionP(int xpos, int ypos);
  bool cCol();
  bool cColP();
  void uCol();
  void back_to_prior_position(float x, float y);
  Point get_player_position();
  void add_block(std::shared_ptr<Rectangle> rectangle);
  Point get_camera_position();
  inline bool running() { return m_is_running; };
  inline float get_delta_time() const { return m_delta_time; }

  SDL_Renderer* m_renderer;
  std::vector<ColliderComponent*> m_colliders;
  SDL_Event event;
  SDL_Rect camera;
private:
  bool m_is_running;
  SDL_Window* m_window;
  std::unique_ptr<Level> m_level;
  float m_delta_time = 0.0f;
};

#endif // GAME_HPP