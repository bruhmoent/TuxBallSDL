#include "level_data.hpp"
#include "game.hpp"

#include <fstream>
#include <sstream>

std::vector<Rectangle*>
LevelData::load_level(std::string path, int sizeX, int sizeY)
{
  std::vector<Rectangle*> tiles;
  std::ifstream level_file(path);
  if (!level_file.is_open()) {
    std::cerr << "Failed to open map file: " << path << std::endl;
    return tiles;
  }

  std::string line;
  int y = 0;
  while (std::getline(level_file, line) && y < sizeY) {
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
        int tile = std::stoi(cell);
        int srcX = (tile % 10) * 32;
        int srcY = (tile / 10) * 32;

        Game::AddTile(srcX, srcY, x * 64, y * 64, x, y, tile);

        if (tile == 0) {
          Rectangle* rect = new Rectangle;
          rect->x = x * 64;
          rect->y = y * 64;
          rect->w = 64;
          rect->h = 64;
          tiles.push_back(rect);
        }
      }
      x++;
    }
    y++;
  }
  level_file.close();

  return tiles;
}

std::vector<Rectangle*>
LevelData::dynamic_load(std::vector<Rectangle*> blocks, int sizeX, int sizeY)
{
  Rectangle* rect = new Rectangle;
  rect->x = sizeX * 64;
  rect->y = sizeY * 64;
  rect->w = 64;
  rect->h = 64;
  blocks[sizeX] = rect;
  return blocks;
}