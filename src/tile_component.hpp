#pragma once
#include "ECS/ECS.h"
#include <SDL3/SDL.h>
#include <iostream>

class TileComponent : public Component
{
public:
  SDL_Texture* texture;
  SDL_FRect srcRect, destRect;
  Vector2D position;

  TileComponent() = default;
  ~TileComponent() {}

  TileComponent(int srcX, int srcY, int xpos, int ypos, SDL_Texture* sharedTex)
  {
    texture = sharedTex;
    position.x = (float)xpos;
    position.y = (float)ypos;

    srcRect.x = srcX;
    srcRect.y = srcY;
    srcRect.w = 32;
    srcRect.h = 32;

    if (texture) {
      float texW = 0, texH = 0;
      SDL_GetTextureSize(texture, &texW, &texH);

      if (srcRect.x + srcRect.w > texW)
        srcRect.x = 0;
      if (srcRect.y + srcRect.h > texH)
        srcRect.y = 0;
    }

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 64;
    destRect.h = 64;
  }

  void update(float dt) override
  {
    destRect.x = (int)position.x - Game::get().camera.x;
    destRect.y = (int)position.y - Game::get().camera.y;
  }

  void draw() override
  {
    if (texture) {
      TextureManager::Draw(texture, srcRect, destRect);
    } else {
      SDL_SetRenderDrawColor(Game::get().m_renderer, 255, 0, 255, 255);
      SDL_RenderFillRect(Game::get().m_renderer, &destRect);
    }
  }
};