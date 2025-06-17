#pragma once
class Point
{
public:
	Point() { x = 0; y = 0; }
	Point(int xy) { x = xy; y = xy; }
	Point(int x, int y) : x(x), y(y) {}

	Point(const Point& point) : x(point.x), y(point.y) {}

	~Point() {}

	int x = 0;
	int y = 0;
};