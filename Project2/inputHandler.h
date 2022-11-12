#pragma once
#include "game.hpp"
#include "ECS/ECS.h"
#include "ECS/Components.h"
#include <stdlib.h>
#include <iostream>
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
	bool jump = false;

	void init()override
	{
		transform = &entity->getComponent<TransformComponent>();
	}
	void update() override
	{
		bool hasCollision = Game::HasCollision(transform->position.x, transform->position.y);
		//transform->position.y++;
		if (!hasCollision) {
			psOld = Game::GetPlayerPosition();
		}


		if (jump) {
			Point* ps3 = Game::GetPlayerPosition();
			if (psOldBeforeJump != NULL && psOldBeforeJump->y > transform->position.y) {
				
			}
			else {
				jump = false;
			}

		}
		else {
			psOldBeforeJump = Game::GetPlayerPosition();
		}

		if (Game::event.type == SDL_KEYDOWN) {

			hasCollision = Game::HasCollision(transform->position.x, transform->position.y);

			if (hasCollision) {
				transform->velocity.x = 0;
				//transform->velocity.y = 0;
			}

			switch (Game::event.key.keysym.sym) {
			/*case SDLK_w:
				hasCollision = Game::HasCollision(transform->position.x, transform->position.y - 5);
				if (hasCollision) {
					transform->velocity.y = 0;

					break;
				}
				else {
					transform->velocity.y = -1;
				}

				hasCollision = Game::HasCollision(transform->position.x, transform->position.y - 1);

				break;*/

			case SDLK_a:
				hasCollision = Game::HasCollision(transform->position.x - 5, transform->position.y);

				if (hasCollision) {
					transform->velocity.x = 0;

					break;
				}
				else {
					transform->velocity.x = transform->velocity.x ? -1 : -1;
					transform->velocity.x = jump ? -1 : -1;
				//	transform->velocity.x = -1;
				}

				hasCollision = Game::HasCollision(transform->position.x + 5, transform->position.y);

				//transform->velocity.x = -1;
				break;
			//case SDLK_s:
			//	transform->velocity.y = 1;
				//break;
			case SDLK_d:
				hasCollision = Game::HasCollision(transform->position.x + 5, transform->position.y);

				if (hasCollision) {
					transform->velocity.x = 0;

					break;
				}
				else {
					transform->velocity.x = transform->velocity.x ? 1 : 1;
					transform->velocity.x = jump ? 1 : 1;
				}

				hasCollision = Game::HasCollision(transform->position.x + 5, transform->position.y);

				break;
			case SDLK_SPACE:
				if (!jump) {
					transform->position.y -= 70;
				}
				jump = true;
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;

				break;
			case SDLK_a:
				transform->velocity.x = 0;
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				break;
			default:

				break;
			}
		}
		//hasCollision = Game::HasCollision(transform->position.x, transform->position.y - 1);
		//if (hasCollision == false)
		//	transform->position.y++;

		if (Game::cCol()) {
			Point* ps2 = Game::GetPlayerPosition();
			//transform->velocity.x = 0;
			//transform->velocity.y = 0;

			if (psOld != NULL) {
				//Game::backToPriorPosition(psOld->x, psOld->y);
				transform->position.x = psOld->x;
				transform->position.y = psOld->y;
			}
			
			return;
		}
		else if (!Game::cColP()) {
			transform->position.y += 2;
		}


	}
};