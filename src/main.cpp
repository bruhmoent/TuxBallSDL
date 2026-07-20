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

#include <memory>

#include "game.hpp"

int
main(int argc, char* argv[])
{
  constexpr int FPS = 120;
  const int frame_delay = 1000 / FPS;
  Uint32 frame_start;
  int frame_time;

  Game& game = Game::get();
  game.init(
    "TuxBall", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

  Uint64 last_time = SDL_GetTicksNS();

  while (game.running()) {
    frame_start = SDL_GetTicks();

    Uint64 current_time = SDL_GetTicksNS();
    float dt = (float)(current_time - last_time) / 1000000000.0f;
    last_time = current_time;

    if (dt > 0.1f) {
      dt = 0.1f;
    }

    game.handle_events();
    game.update(dt);
    game.render();

    frame_time = SDL_GetTicks() - frame_start;

    if (frame_delay > frame_time) {
      SDL_Delay(frame_delay - frame_time);
    }
  }

  game.clean();
  return 0;
}