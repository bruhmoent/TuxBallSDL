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
#include<chrono>

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


		bool hasCollision = Game::HasCollision(transform->position.x, transform->position.y);
		//transform->position.y++;
		if (!hasCollision) {
			psOld = Game::GetPlayerPosition();
		}

		timeC = getTime();
	
		long timeF = getDurationInMilisecons(timeOld, timeC);

		if (timeF > 0 && jump)
		{
			//std::cout << "C: " << timeF << " ";

			if (timeF > 0 && timeF <= 300) {
				//70px -> 300ms
				//x -> timeF ms
				transform->position.y -= 30 * timeF / 600;
			}
			else if (timeF > 300 && timeF < 40)
			{
				//	transform->velocity.y += 1;
			}
			else {
				//	transform->velocity.y = -1;
			}

			if (timeF >= 400 || timeF == 0) {
				jump = false;
			//	std::cout << "Koniec skoku ";
				//transform->velocity.y = 0;
				//std::cout << getDurationInMilisecons(timeOld, timeC) << " ";
			}
		}

		if (jump) {

			Point* ps3 = Game::GetPlayerPosition();
			if (psOldBeforeJump != NULL && psOldBeforeJump->y > transform->position.y) {
			}

			

		}
		else {
			psOldBeforeJump = Game::GetPlayerPosition();
		}

		/*	if (jump) {
				long timeF = getDurationInMilisecons(timeOld, timeC);
				transform->position.y -= 70 * timeF / 650;
			}*/




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
					transform->velocity.x = transform->velocity.x ? -1 : -2;
					transform->velocity.x = jump ? -1 : -2;
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
					transform->velocity.x = transform->velocity.x ? 1 : 2;
					transform->velocity.x = jump ? 1 : 2;
				}

				hasCollision = Game::HasCollision(transform->position.x + 5, transform->position.y);

				break;
			case SDLK_SPACE:
				if (!jump) {
					timeOld = getTime();
					timeC = getTime();
					//transform->position.y -= 70;
					//std::cout << "Skok "<<timeOld<<" ";
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

				auto velMirror = transform-> velocity * -1;
				transform->velocity = velMirror;
	
				//transform->velocity = velMirror;
			}

			return;
		}
		else if (!Game::cColP()) {
			auto q = 2 + timeF / 650;
			transform->position.y += q;
		
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