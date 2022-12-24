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
#define Game_hpp
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Point.h"
#include "Rectagle.h"

class ColliderComponent;
class Game {
public:
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static void AddTile(int srcX, int srcY, int xpos, int ypos, int x, int y, int kind);
	static bool HasCollision(int xpos, int ypos);
	static bool HasCollisionP(int xpos, int ypos);
	static SDL_Rect camera;
	static bool cCol();
	static bool cColP();
	static void uCol();
	static void backToPriorPosition(float x, float y);
	static Point* GetPlayerPosition();
	static void AddBlock(Rectagle* rectangle);
	static Point* GetCameraPosition();
	bool running() {
		return isRunning;
	};

private:
	bool isRunning;
	SDL_Window* window;
};	