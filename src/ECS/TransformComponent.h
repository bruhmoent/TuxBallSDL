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
#include "Components.h"
#include "../vectorHandler.h"

class TransformComponent : public Component
{

public:
	Vector2D position;
	Vector2D velocity;
	int height = 32;
	int width = 32;
	int scale = 1;
	int speed = 3;

	TransformComponent()
	{
		position.Zero();
	}
	TransformComponent(int sc)
	{
		position.Zero();
		scale = sc;
	}
	TransformComponent(float x, float y)
	{
		position.Zero();
	}
	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}
	void init() override
	{
		velocity.Zero();
	}
	void update()override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};