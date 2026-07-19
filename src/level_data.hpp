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

#ifndef LEVEL_DATA_HPP
#define LEVEL_DATA_HPP

#include <string>
#include <vector>
#include <memory>

#include "rectangle.hpp"

class LevelData
{
public:
  static std::vector<std::shared_ptr<Rectangle>> load_level(std::string path,
                                                            int sizeX,
                                                            int sizeY);
  static std::vector<std::shared_ptr<Rectangle>> dynamic_load(
    std::vector<std::shared_ptr<Rectangle>> blocks,
    int sizeX,
    int sizeY);
};

#endif // LEVEL_DATA_HPP