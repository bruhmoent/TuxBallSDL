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

#pragma once
#include "ECS/Components.h"
#include "ECS/ECS.h"
#include "game.hpp"
#include "level_data.hpp"
#include "rectangle.hpp"
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

class KeyboardController : public Component
{
public:
  char zero = '0';
  LevelData* map;
  TransformComponent* transform;

  Point old_position{ 0.0f, 0.0f };
  bool has_old_position = false;
  Point old_position_before_jump{ 0.0f, 0.0f };

  long current_time = 0;
  long old_time = 0;
  bool is_duration_active = false;
  int block_x = 0, block_y = 0;
  bool is_jumping = false;

  void init() override
  {
    transform = &entity->getComponent<TransformComponent>();
  }

  void update() override
  {
    int camera_x = int(Game::get().GetCameraPosition().x);
    int camera_y = int(Game::get().GetCameraPosition().y);

    if (Game::get().cCol()) {
      if (has_old_position) {
        transform->position.x = old_position.x;
        if (!is_duration_active) {
          transform->position.y = old_position.y - 0.5f;
        } else {
          transform->position.y = old_position.y + 0.5f;
        }
        auto vel_mirror = transform->velocity.x * -1.0f;
        transform->velocity.x = vel_mirror / 1.5f;
      }
      return;
    }

    bool has_collision = Game::get().HasCollision((int)transform->position.x,
                                                  (int)transform->position.y);
    bool has_platform_collision = Game::get().HasCollisionP(
      (int)transform->position.x, (int)transform->position.y);

    if (has_collision) {
      transform->velocity.x *= 0.7f;
      if (transform->velocity.x < 0.001f)
        transform->velocity.x = 0.0f;
    }

    bool overwrite = false;
    if (!has_collision) {
      old_position = Game::get().GetPlayerPosition();
      has_old_position = true;
    }

    current_time = get_time_ms();
    long time_delta = get_duration_in_milliseconds(old_time, current_time);

    if (has_old_position ? has_collision : true) {
      overwrite = true;
    }

    if (!overwrite && time_delta > 0 && is_jumping) {
      if (time_delta > 0 && time_delta <= 300) {
        transform->position.y -= 30.0f * time_delta / 600.0f;
      }
      if (time_delta >= 400 || time_delta == 0) {
        is_jumping = false;
      }
    }

    if (is_jumping) {
      is_duration_active = true;
      if (has_collision) {
        transform->position.y += 30.0f * time_delta / 600.0f;
        is_jumping = false;
      }
    } else {
      old_position_before_jump = Game::get().GetPlayerPosition();
      is_duration_active = false;
    }

    if (Game::get().event.type == SDL_EVENT_KEY_DOWN) {
      if (has_collision) {
        transform->velocity.x = 0.0f;
      }
      switch (Game::get().event.key.key) {
        case SDLK_A:
          has_collision = Game::get().HasCollision(
            (int)transform->position.x - 5, (int)transform->position.y);
          if (has_collision) {
            transform->velocity.x = 0.0f;
            break;
          } else {
            transform->velocity.x = -2.0f * time_delta;
            transform->velocity.x = is_jumping ? -1.0f : -2.0f;
          }
          break;
        case SDLK_D:
          has_collision = Game::get().HasCollision(
            (int)transform->position.x + 5, (int)transform->position.y);
          if (has_collision) {
            transform->velocity.x = 0.0f;
            break;
          } else {
            transform->velocity.x = 2.0f * time_delta;
            transform->velocity.x = is_jumping ? 1.0f : 2.0f;
          }
          break;
        case SDLK_SPACE:
          if (!has_collision) {
            is_jumping = false;
          }
          if (!is_jumping && !Game::get().cCol()) {
            old_time = get_time_ms();
            current_time = get_time_ms();
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

          if (std::abs(Game::get().GetPlayerPosition().y -
                       (camera_y + (int)mouse_y - (int)mouse_y % 64)) > 70 ||
              std::abs(Game::get().GetPlayerPosition().x -
                       (camera_x + (int)mouse_x - (int)mouse_x % 64)) > 70) {

            Game::get().AddTile(src_x,
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
            Game::get().AddBlock(new_rect);
          }
          break;
        }
        // To do: for SDLK_y make the player able to remove blocks.
        default:
          break;
      }
    }

    if (Game::get().event.type == SDL_EVENT_KEY_UP) {
      if (!Game::get().cCol()) {
        switch (Game::get().event.key.key) {
          case SDLK_W:
          case SDLK_S:
            transform->velocity.y = 0.0f;
            break;
          case SDLK_A:
          case SDLK_D:
            transform->velocity.x = 0.0f;
            break;
          default:
            break;
        }
      }
    } else if (!Game::get().cColP()) {
      auto gravity = 3 + time_delta / 650;
      transform->position.y += gravity;
    }
  }

  long get_time_ms()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
  }

  long get_duration_in_milliseconds(long start, long stop)
  {
    if (start <= 0 || stop <= 0 || start >= stop) {
      return 0;
    }
    return (stop - start);
  }
};