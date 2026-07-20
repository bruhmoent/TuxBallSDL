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

#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "ECS/ECS.h"
#include "game.hpp"
#include "level_data.hpp"
#include "rectangle.hpp"
#include <cmath>
#include <vector>

class KeyboardController : public Component
{
public:
  char zero = '0';
  TransformComponent* transform;

  bool is_jumping = false;

  const float GRAVITY = 25.0f;
  const float JUMP_FORCE = -5.0f;
  const float MOVE_SPEED = 2.0f;

  void init() override
  {
    transform = &entity->getComponent<TransformComponent>();
  }

  void update(float dt) override
  {
    int camera_x = int(Game::get().get_camera_position().x);
    int camera_y = int(Game::get().get_camera_position().y);

    const bool* key_state = SDL_GetKeyboardState(NULL);

    transform->velocity.x = 0.0f;
    if (key_state[SDL_SCANCODE_A]) {
      transform->velocity.x = -MOVE_SPEED;
    }
    if (key_state[SDL_SCANCODE_D]) {
      transform->velocity.x = MOVE_SPEED;
    }

    bool has_platform_collision = Game::get().cColP();
    const float TERMINAL_VELOCITY = 15.0f;

    if (!has_platform_collision) {
      transform->velocity.y += GRAVITY * dt;
      if (transform->velocity.y > TERMINAL_VELOCITY) {
        transform->velocity.y = TERMINAL_VELOCITY;
      }
    } else {
      is_jumping = false;
      if (transform->velocity.y > 0) {
        transform->velocity.y = 0.0f;
      }
    }

    if (Game::get().event.type == SDL_EVENT_KEY_DOWN) {
      switch (Game::get().event.key.key) {
        case SDLK_SPACE:
          if (has_platform_collision && !is_jumping) {
            transform->velocity.y = JUMP_FORCE;
            is_jumping = true;
          }
          break;

        case SDLK_X: {
          int block_id = zero - '0';
          int src_x = (block_id % 10) * 32;
          int src_y = (block_id / 10) * 32;
          float mouse_x, mouse_y;
          SDL_GetMouseState(&mouse_x, &mouse_y);
          camera_x -= camera_x % 64;
          camera_y -= camera_y % 64;

          if (std::abs(Game::get().get_player_position().y -
                       (camera_y + (int)mouse_y - (int)mouse_y % 64)) > 70 ||
              std::abs(Game::get().get_player_position().x -
                       (camera_x + (int)mouse_x - (int)mouse_x % 64)) > 70) {

            Game::get().add_tile(src_x,
                                 src_y,
                                 camera_x + (int)mouse_x - (int)mouse_x % 64,
                                 camera_y + (int)mouse_y - (int)mouse_y % 64,
                                 (int)mouse_x / 64,
                                 (int)mouse_y / 64,
                                 0);

            auto new_rect = std::make_shared<Rectangle>();
            new_rect->x = camera_x + (int)mouse_x - (int)mouse_x % 64;
            new_rect->y = camera_y + (int)mouse_y - (int)mouse_y % 64;
            new_rect->w = 64;
            new_rect->h = 64;
            Game::get().add_block(new_rect);
          }
          break;
        }
        default:
          break;
      }

      Game::get().event.type = 0;
    }
  }
};

#endif // INPUT_HANDLER_HPP