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

#include "tilemapObj.h"
#include "game.hpp"
#include <fstream>
#include <iostream>

Map::Map(){};
Map::~Map() {}
std::vector<Rectagle*> Map::loadMap(std::string path, int sizeX, int sizeY)
{
	std::vector<Rectagle*> blocks;
	char c;
	std::fstream mapFile;
	mapFile.open(path);
	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{

			mapFile.get(c);
			int block = atoi(&c);
			srcY = atoi(&c) * 32;
			srcX = atoi(&c) * 32;

			Game::AddTile(srcX, srcY, x * 64, y * 64, x, y, atoi(&c));

			if (block == 0) {
				Rectagle* rect = new Rectagle;
				rect->x = x * 64;
				rect->y = y * 64;
				rect->w = 64;
				rect->h = 64;
				blocks.push_back(rect);
			}
			mapFile.ignore();

		}

	}
	mapFile.close();

	return blocks;
};
std::vector<Rectagle*> Map::dynamicLoad(std::vector<Rectagle*> blocks, int sizeX, int sizeY)
{
		Rectagle* rect = new Rectagle;
		rect->x = sizeX * 64;
		rect->y = sizeY * 64;
		rect->w = 64;
		rect->h = 64;
		blocks[sizeX] = rect;
	return blocks;
}
