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
  void init(const char* title,
            int xpos,
            int ypos,
            int width,
            int height,
            bool fullscreen);

  void handleEvents();
  void update();
  void render();
  void clean();
  SDL_Event event;
  void AddTile(int srcX, int srcY, int xpos, int ypos, int x, int y, int kind);
  bool HasCollision(int xpos, int ypos);
  bool HasCollisionP(int xpos, int ypos);
  SDL_Rect camera;
  bool cCol();
  bool cColP();
  void uCol();
  void backToPriorPosition(float x, float y);
  Point GetPlayerPosition();
  void AddBlock(std::shared_ptr<Rectangle> rectangle);
  Point GetCameraPosition();
  inline bool running() { return m_is_running; };

  SDL_Renderer* m_renderer;
  std::vector<ColliderComponent*> m_colliders;
private:
  bool m_is_running;
  SDL_Window* m_window;
  std::unique_ptr<Level> m_level;
};

#endif // GAME_HPP