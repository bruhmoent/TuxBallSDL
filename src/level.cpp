#include <iostream>
#include <vector>

#include <SDL3/SDL.h>

#include "config.hpp"
#include "level.hpp"
#include "rectangle.hpp"
#include "texture_manager.hpp"

Level::Level()
  : m_data(std::make_unique<LevelData>())
{
  tileset_texture = nullptr;
}

void
Level::load()
{
  const std::string m_tileset_path =
    (std::filesystem::path(Config::get_project_root()) / "assets" / "images" /
     "tileTs.bmp")
      .string();

  tileset_texture = TextureManager::LoadTexture(m_tileset_path.c_str());
  if (!tileset_texture) {
    std::cerr << "CRITICAL: Failed to load tileset texture!" << std::endl;
  }

  const std::string level_file =
    (std::filesystem::path(Config::get_project_root()) / "assets" / "levels" /
     "map3.txt")
      .string();

  tiles = LevelData::load_level(level_file, 25, 20);
}

Level::~Level()
{
  SDL_DestroyTexture(tileset_texture);
}