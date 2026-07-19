#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <memory>

#include "level_data.hpp"

class Level
{
public:
  Level();
  ~Level();

  void load();
  std::vector<std::shared_ptr<Rectangle>> tiles;
  SDL_Texture* tileset_texture;
private:
  std::unique_ptr<LevelData> m_data;
  std::string m_tileset_path;
};

#endif // LEVEL_HPP