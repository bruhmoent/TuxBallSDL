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

#include "vectorHandler.h"

Vector2D::Vector2D(){ x = 0.0f; y = 0.0f; }

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D& Vector2D::Add(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return *this;
};
Vector2D& Vector2D::Substract(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
};
Vector2D& Vector2D::Multiply(const Vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	return *this;
};
Vector2D& Vector2D::Divide(const Vector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	return *this;
};

Vector2D& Vector2D::operator*(const int& i)
{
	this->x *= i;
	this->y *= i;
	return *this;
}

Vector2D& Vector2D::Zero()
{
	this->x = 0;
	this->y = 0;
	return *this;
}
Vector2D& operator+(Vector2D& v1, const Vector2D& v2)
{
	return v1.Add(v2);
};

Vector2D& operator-(Vector2D& v1, const Vector2D& v2)
{
	return v1.Substract(v2);
};

Vector2D& operator*(Vector2D& v1, const Vector2D& v2)
{
	return v1.Multiply(v2);
};

Vector2D& operator/(Vector2D& v1, const Vector2D& v2)
{
	return v1.Divide(v2);
};

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->Add(vec);
};
Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->Substract(vec);
};
Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	return this->Multiply(vec);
};
Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	return this->Divide(vec);
};

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.x << ", " << vec.y << ")";
	return stream;
};