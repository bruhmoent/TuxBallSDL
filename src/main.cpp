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

  auto game = std::make_unique<Game>();
  game->init(
    "TuxBall", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

  while (game->running()) {
    frame_start = SDL_GetTicks();

    game->handleEvents();
    game->update();
    game->render();

    frame_time = SDL_GetTicks() - frame_start;

    if (frame_delay > frame_time) {
      SDL_Delay(frame_delay - frame_time);
    }
  }

  game->clean();
  return 0;
}