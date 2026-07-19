#include "tilemap_object.hpp"
#include "game.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Map::Map() {};
Map::~Map() {}

std::vector<Rectagle*>
Map::loadMap(std::string path, int sizeX, int sizeY)
{
  std::vector<Rectagle*> blocks;
  std::ifstream mapFile(path);
  if (!mapFile.is_open()) {
    std::cerr << "Failed to open map file: " << path << std::endl;
    return blocks;
  }

  std::string line;
  int y = 0;
  while (std::getline(mapFile, line) && y < sizeY) {
    if (line.empty())
      continue;

    std::stringstream ss(line);
    std::string cell;
    int x = 0;

    while (std::getline(ss, cell, ',') && x < sizeX) {
      if (!cell.empty() && cell.back() == '\r') {
        cell.pop_back();
      }

      if (!cell.empty()) {
        int block = std::stoi(cell);
        int srcX = (block % 10) * 32;
        int srcY = (block / 10) * 32;

        Game::AddTile(srcX, srcY, x * 64, y * 64, x, y, block);

        if (block == 0) {
          Rectagle* rect = new Rectagle;
          rect->x = x * 64;
          rect->y = y * 64;
          rect->w = 64;
          rect->h = 64;
          blocks.push_back(rect);
        }
      }
      x++;
    }
    y++;
  }
  mapFile.close();

  return blocks;
}

std::vector<Rectagle*>
Map::dynamicLoad(std::vector<Rectagle*> blocks, int sizeX, int sizeY)
{
  Rectagle* rect = new Rectagle;
  rect->x = sizeX * 64;
  rect->y = sizeY * 64;
  rect->w = 64;
  rect->h = 64;
  blocks[sizeX] = rect;
  return blocks;
}