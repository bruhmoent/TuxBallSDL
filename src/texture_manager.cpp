#include "texture_manager.hpp"
#include <iostream>

SDL_Texture*
TextureManager::LoadTexture(const char* texture)
{
  SDL_Surface* tempSurface = IMG_Load(texture);
  if (!tempSurface) {
    std::cout << "IMG_Load Error (" << texture << "): " << SDL_GetError()
              << std::endl;
    return nullptr;
  }

  SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
  SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_LINEAR);
  SDL_DestroySurface(tempSurface);

  return tex;
}

void
TextureManager::Draw(SDL_Texture* tex, SDL_FRect src, SDL_FRect dest)
{
  if (tex) {
    if (!SDL_RenderTexture(Game::renderer, tex, &src, &dest)) {
      std::cout << "Render Error: " << SDL_GetError() << std::endl;
    }
  }
}