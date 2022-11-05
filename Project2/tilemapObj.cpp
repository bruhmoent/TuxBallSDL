#include "tilemapObj.h"
#include "game.hpp"
#include <fstream>
#include <iostream>
Map::Map()
{

};
Map::~Map() {

}
void Map::loadMap(std::string path, int sizeX, int sizeY)
{
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
			srcY = atoi(&c) * 32;

			srcX = atoi(&c) * 32;


			Game::AddTile(srcX, srcY, x * 64, y * 64, x, y, atoi(&c));
			mapFile.ignore();

		}

	}
	mapFile.close();
};
