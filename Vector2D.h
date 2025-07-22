#pragma once

class Vector2D
{
	public:
		Vector2D() { x = 0; y = 0; }
		Vector2D(float xy) { x = xy; y = xy; }
		Vector2D(float x, float y) : x(x), y(y) {}

		Vector2D(const Vector2D& point) : x(point.x), y(point.y) {}

		~Vector2D() {}
		Vector2D operator+(const Vector2D& other) const
		{
			return Vector2D(x + other.x, y + other.y);
		}
		Vector2D operator-(const Vector2D& other) const
		{
			return Vector2D(x - other.x, y - other.y);
		}
		Vector2D operator*(float scalar) const
		{
			return Vector2D(x * scalar, y * scalar);
		}
		Vector2D operator/(float scalar) const
		{
			if (scalar != 0)
				return Vector2D(x / scalar, y / scalar);
			else
				return Vector2D(0, 0); // Avoid division by zero
		}
		Vector2D& operator+=(const Vector2D& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2D& operator-=(const Vector2D& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vector2D& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}
		Vector2D& operator/=(float scalar)
		{
			if (scalar != 0)
			{
				x /= scalar;
				y /= scalar;
			}
			else
			{
				x = 0;
				y = 0; // Avoid division by zero
			}
			return *this;
		}



		float x = 0;
		float y = 0;
};