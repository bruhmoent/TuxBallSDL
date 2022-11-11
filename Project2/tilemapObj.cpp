#include "tilemapObj.h"
#include "game.hpp"
#include <fstream>
#include <iostream>
Map::Map()
{

};
Map::~Map() {

}
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
			std::cout << "" << c << " ";
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
