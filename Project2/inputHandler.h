#pragma once
#include "game.hpp"
#include "ECS/ECS.h"
#include "ECS/Components.h"
#include <time.h>
#include <windows.h>
#include <vector>
#include <chrono>
#include <thread>
class KeyboardController : public Component
{
public:
	TransformComponent* transform;
	Point* psOld;
	Point* psOldBeforeJump;
	long timeC;
	long timeOld;
	bool jump = false;
	void init()override
	{
		transform = &entity->getComponent<TransformComponent>();
	}
	void update() override
	{
		if (Game::cCol()) {
			Point* ps2 = Game::GetPlayerPosition();
			if (psOld != NULL) {
				transform->position.x = psOld->x;
				transform->position.y = psOld->y;
				auto velMirror = transform->velocity.x * -1.0f;
				transform->velocity.x = velMirror / 1.5f;
			}
			return;
		}
		bool hasCollision = Game::HasCollision(transform->position.x, transform->position.y);
		bool hasCollisionP = Game::HasCollisionP(transform->position.x, transform->position.y);
		if (hasCollision) {
			transform->velocity.x = 0.0f;
			if(transform->velocity.x >= 1)
		    transform->velocity.y = 0.0f;
		}
		bool overWrite = false;
		if (!hasCollision) {
			psOld = Game::GetPlayerPosition();
		}
		timeC = getTime();
		long timeF = getDurationInMilisecons(timeOld, timeC);
		if (psOld + 3? hasCollision : true)
		{
			overWrite = true;
		}
		if (!overWrite && timeF > 0 && jump)
		{
			if (timeF > 0 && timeF <= 300) {
				transform->position.y -= 30.0f  * timeF / 600.0f;
			}
			if (timeF >= 400 || timeF == 0) {
				jump = false;
			}
		}
		if (jump) {
			Point* ps3 = Game::GetPlayerPosition();
			if (hasCollision)
			{
				transform->position.y += 30.0f * timeF / 600.0f;
				jump = false;
			}
		}
		else {
			psOldBeforeJump = Game::GetPlayerPosition();
		}
		if (Game::event.type == SDL_KEYDOWN) {
			if (hasCollision) {
				transform->velocity.x = 0.0f;
			}
			switch (Game::event.key.keysym.sym) {
			case SDLK_a:
				hasCollision = Game::HasCollision(transform->position.x - 5, transform->position.y);
				if (hasCollision) {
					transform->velocity.x = 0.0f;
					break;
				}
				else {
					transform->velocity.x = transform->velocity.x ? -1.0f : -2.0f;
					transform->velocity.x = jump ? -1.0f : -2.0f;
				}
				break;
			case SDLK_d:
				hasCollision = Game::HasCollision(transform->position.x + 5, transform->position.y);
				if (hasCollision) {
					transform->velocity.x = 0.0f;
					break;
				}
				else {
					transform->velocity.x = transform->velocity.x ? 1.0f : 2.0f;
					transform->velocity.x = jump ? 1.0f : 2.0f;
				}
				break;
			case SDLK_SPACE:
				if (!hasCollision)
				{
					jump = false;
				}
				if (!jump && !Game::cCol()) {
					timeOld = getTime();
					timeC = getTime();
					jump = true;
				}
				break;
			default:
				break;
			}
		}
		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym) {
				if (!Game::cCol()) {
			case SDLK_w:
			case SDLK_s:
				transform->velocity.y = 0.0f;
				break;
			case SDLK_a:
			case SDLK_d:
				transform->velocity.x = 0.0f;
				break;
			default:
				break;
				}
			}
		}
		else if (!Game::cColP()) {
			auto pGravity = 3 + timeF / 650;
			transform->position.y += pGravity;
		}
	}
	long getTime()
	{
		auto t = std::chrono::high_resolution_clock::now();
		long timeS = duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		return timeS;
	}
	long getDurationInMilisecons(long start, long stop) {
		if (start <= 0 || stop <= 0 || start >= stop)
		{
			return 0;
		}
		return (stop - start);
	}
};
