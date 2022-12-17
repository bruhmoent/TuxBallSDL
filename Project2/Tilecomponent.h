#pragma once
#include "ECS/ECS.h"
#include "SDL.h"

class TileComponent : public Component
{
public:

	SDL_Texture* texture;

	SDL_Rect srcRect, destRect;
	Vector2D position;
	TileComponent() = default;

	~TileComponent() {
		SDL_DestroyTexture(texture);
	}
	TileComponent(int srcX, int srcY, int xpos, int ypos, const char* path)
	{
		texture = TextureManager::LoadTexture(path);
		position.x = (float)xpos;
		position.y = (float)ypos;
		srcRect.x = srcX;
		srcRect.y = srcY;

		srcRect.w = srcRect.h = 32;

		destRect.x = xpos;
		destRect.y = ypos;

		destRect.w = destRect.h = 64;
	}

	void update()override
	{
		destRect.x = (int)position.x - Game::camera.x;
		destRect.y = (int)position.y - Game::camera.y;
	}
	void draw()override
	{
		TextureManager::Draw(texture, srcRect, destRect);
	}
};