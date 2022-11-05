#pragma once
#include "SDL.h"
class ColliderComponent;
class Collision
{
public:
	static bool AABB(const SDL_Rect& rectA, SDL_Rect& rectB);
	static bool AABB(const ColliderComponent& colA, ColliderComponent& colB);

};