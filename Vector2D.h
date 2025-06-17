#pragma once

class Vector2D
{
	public:
		Vector2D() { x = 0; y = 0; }
		Vector2D(float xy) { x = xy; y = xy; }
		Vector2D(float x, float y) : x(x), y(y) {}

		Vector2D(const Vector2D& point) : x(point.x), y(point.y) {}

		~Vector2D() {}

		float x = 0;
		float y = 0;
};