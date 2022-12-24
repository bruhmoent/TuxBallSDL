//  TuxBall
//  Copyright (C) 2022 bruhmoent
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once
#include "game.hpp"
#include "ECS/ECS.h"
#include "tilemapObj.h"
#include "ECS/Components.h"
#include <time.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include "Rectagle.h"
#include <cmath>

class KeyboardController : public Component
{
public:
	Rectagle* rectangle;

	char zero = '0';
	Map* map;
	TransformComponent* transform;
	Point* psOld;
	Point* psOldBeforeJump;
	long timeC;
	long timeOld;
	bool duration;
	int blockX, blockY = 0;
	bool jump = false;
	void init()override
	{
		transform = &entity->getComponent<TransformComponent>();
	}
	void update() override
	{
		int cameraX = int(Game::GetCameraPosition()->x);
		int cameraY = int(Game::GetCameraPosition()->y);
		uint32_t currentTime = SDL_GetTicks();
		if (Game::cCol()) {
			Point* ps2 = Game::GetPlayerPosition();
			if (psOld != NULL) {
				transform->position.x = psOld->x;
				if (!duration)	{ transform->position.y = psOld->y - 0.5f; }
				else { transform->position.y = psOld->y + 0.5f; }
				auto velMirror = transform->velocity.x * -1.0f;
				transform->velocity.x = velMirror / 1.5f;
			}
			return;
		}
		bool hasCollision = Game::HasCollision((int)transform->position.x, (int)transform->position.y);
		bool hasCollisionP = Game::HasCollisionP((int)transform->position.x, (int)transform->position.y);
		if (hasCollision) {
			transform->velocity.x *= 0.7f;
			if (transform->velocity.x < 0.001f)
				transform->velocity.x = 0.0f;
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
			duration = true;
			Point* ps3 = Game::GetPlayerPosition();
			if (hasCollision)
			{
				transform->position.y += 30.0f * timeF / 600.0f;
				jump = false;
			}
		}
		else {
			psOldBeforeJump = Game::GetPlayerPosition();
			duration = false;
		}
		if (Game::event.type == SDL_KEYDOWN) {
			if (hasCollision) {
				transform->velocity.x = 0.0f;
			}
			switch (Game::event.key.keysym.sym) {
			case SDLK_a:
				hasCollision = Game::HasCollision((int)transform->position.x - 5, (int)transform->position.y);
				if (hasCollision) {
					transform->velocity.x = 0.0f;
					break;
				}
				else {
					transform->velocity.x = -2.0f*timeF;
					transform->velocity.x = jump ? -1.0f : -2.0f;
				}
				break;
			case SDLK_d:
				hasCollision = Game::HasCollision((int)transform->position.x + 5, (int)transform->position.y);
				if (hasCollision) {
					transform->velocity.x = 0.0f;
					break;
				}
				else {
					transform->velocity.x = 2.0f * timeF;
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
			case SDLK_x:
				int mouseX, mouseY;
				int srcX, srcY;
				srcY = atoi(&zero) * 32;
				srcX = atoi(&zero) * 32;
				SDL_GetMouseState(&mouseX, &mouseY);
				cameraX -= cameraX % 64;
				cameraY -= cameraY % 64;

				if (abs(Game::GetPlayerPosition()->y - (cameraY + mouseY - mouseY % 64)) > 70
					|| abs(Game::GetPlayerPosition()->x - (cameraX + mouseX - mouseX % 64)) > 70) {
					Game::AddTile(srcX, srcY, cameraX + mouseX - mouseX % 64, cameraY + mouseY - mouseY % 64, mouseX / 64, mouseY / 64, 0);
					rectangle = new Rectagle();
					rectangle->x = cameraX + mouseX - mouseX % 64;
					rectangle->y = cameraY + mouseY - mouseY % 64;
					rectangle->w = 64;
					rectangle->h = 64;
					Game::AddBlock(rectangle);
				}
			break;
			//To do: for SDLK_y make the player able to remove blocks.
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