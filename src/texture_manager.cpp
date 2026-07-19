#include "texture_manager.hpp"
#include <iostream>

SDL_Texture*
TextureManager::LoadTexture(const char* texture)
{
  SDL_Surface* temp_surface = IMG_Load(texture);
  if (!temp_surface) {
    std::cout << "IMG_Load failed for: " << texture
              << " | Error: " << SDL_GetError() << std::endl;
    return nullptr;
  }

  SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::get().m_renderer, temp_surface);
  if (!tex) {
    std::cout << "SDL_CreateTextureFromSurface failed for: " << texture
              << " | Error: " << SDL_GetError() << std::endl;
  } else {
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_LINEAR);
  }

  SDL_DestroySurface(temp_surface);
  return tex;
}

void
TextureManager::Draw(SDL_Texture* tex, SDL_FRect src, SDL_FRect dest)
{
  if (tex) {
    if (!SDL_RenderTexture(Game::get().m_renderer, tex, &src, &dest)) {
      std::cout << "Render Error: " << SDL_GetError() << std::endl;
    }
  }
}