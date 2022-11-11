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
private:

};