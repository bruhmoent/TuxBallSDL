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
#include <string>
#include <vector>
#include "Rectagle.h"
class Map
{
public:
	Map();
	~Map();
	static std::vector<Rectagle*> loadMap(std::string path, int sizeX, int sizeY);
	static std::vector<Rectagle*> dynamicLoad(std::vector<Rectagle*> blocks, int sizeX, int sizeY);
private:
};