#include "Vector2D.h"

float Vector2d::distance(const Vector2d& other)
{
	return (other - *this).magnitude();
}

float Vector2d::magnitude()
{
	return sqrt((this->x * this->x) + (this->y * this->y));
}

Vector2d Vector2d::operator* (float number) const
{
	return Vector2d((this->x * number), (this->y * number));
}

Vector2d Vector2d::operator/ (float number) const 
{
	return Vector2d((this->x / number), (this->y / number));
}

Vector2d Vector2d::operator+ (float number) const
{
	return Vector2d((this->x + number), (this->y + number));
}

Vector2d Vector2d::operator+ (Vector2d other) const
{
	return Vector2d((this->x + other.x), (this->y + other.y));
}

Vector2d Vector2d::operator- (float number) const
{
	return Vector2d((this->x - number), (this->y - number));
}

Vector2d Vector2d::operator- (Vector2d other) const
{
	return Vector2d((this->x - other.x), (this->y - other.y));
}

bool Vector2d::operator== (const Vector2d& other) const
{
  return ((this->x == other.x) && (this->y == other.y));
}

bool Vector2d::floatEquals(const Vector2d& other) const
{
  return (abs(this->x - other.x) <= 0.000001f && abs(this->y - other.y) <= 0.000001f);
}

void Vector2d::normalize()
{
	this->x /= magnitude();
	this->y /= magnitude();
}

bool Vector2d::isWithinSquareRange(const Vector2d& other, float distance)
{
  if (abs(this->x - other.x) <= (distance/2.f) && abs(this->y - other.y) <= (distance/2.f))
    return true;
  return false;
}

float Vector2d::dotProduct(const Vector2d& other) const
{
	return ((other.x * this->x) + (other.y * this->y));
}
