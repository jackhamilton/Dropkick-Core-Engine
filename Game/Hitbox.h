#pragma once
#include "Rectangle.h"
#include "Circle.h"
#include "Vector.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;
class GameObject;

struct Collision {
	bool collided;
	Point p;
};

class Hitbox
{
public:
	Hitbox(Circle c, Point* parentPosition);
	Hitbox(Rectangle r, Point* parentPosition);
	//UNIMPLEMENTED
	//does not work with rotated rectangles
	bool testCollision(Hitbox h);
	Point getCenter();
	//second return is whether a collision happened
	std::pair<Vector, Collision> getMaximumClearDistanceForVectorFromGameObject(std::vector<shared_ptr<Hitbox>> objects, Vector vector);
	//Rotates point around center of hitbox
	static Point rotate(Point p, double deg);
	vector<Point> getCorners();

	static vector<Point> getEquallySpacedPointsAlongLine(Point origin, Vector a, int pts);
	static bool lineCircle(double x1, double y1, double x2, double y2, double cx, double cy, double r, shared_ptr<Point> nearestCollision);
	static bool lineRect(double x1, double y1, double x2, double y2, double rx, double ry, double rx2, double ry2, shared_ptr<Point> nearestCollision);
	static bool lineLine(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, shared_ptr<Point> collision);
	static bool LeMotheLineLine(double p0_x, double p0_y, double p1_x, double p1_y, double p2_x, double p2_y, double p3_x, double p3_y, double* i_x, double* i_y)
	{
		double s1_x, s1_y, s2_x, s2_y;
		s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
		s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

		double s, t;
		s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
		t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			// Collision detected
			if (i_x != NULL)
				*i_x = p0_x + (t * s1_x);
			if (i_y != NULL)
				*i_y = p0_y + (t * s1_y);
			return true;
		}

		return false; // No collision
	};
	static bool crossProductLineLine(Point a1, Point a2, Point b1, Point b2, Point* result) {
		double denom = ((b2.y - b1.y) * (a2.x - a1.x)) - ((b2.x - b1.x) * (a2.y - a1.y));
		if (denom == 0) {
			return false;
		}
		else {
			double ua = (((b2.x - b1.x) * (a1.y - b1.y)) - ((b2.y - b1.y) * (a1.x - b1.x))) / denom;
				/* The following 3 lines are only necessary if we are checking line
					segments instead of infinite-length lines */
			double ub = (((a2.x - a1.x) * (a1.y - b1.y)) - ((a2.y - a1.y) * (a1.x - b1.x))) / denom;
			if ((ua < 0) || (ua > 1) || (ub < 0) || (ub > 1))
				return false;
			*result = a1 + (a2 - a1) * ua;
			return true;
		}
	}
	
	static bool linePoint(double x1, double y1, double x2, double y2, double px, double py);
	static bool pointCircle(double px, double py, double cx, double cy, double r);
private:
	Point* parentPosition;
	int rectCollisionTestPrecision = 3;
	double rotation;
	bool usesCircleHitbox;
	Circle hitboxCircle;
	Rectangle hitboxRect;
	vector<Point> getRectangularPointsSet();

	//UNIMPLEMENTED FULLY
	static bool testCircleCollision(shared_ptr<Hitbox> h1, shared_ptr<Hitbox> h2);
	//circle first, does not work with rotation
	static bool testHybridCollision(shared_ptr<Hitbox> h1, shared_ptr<Hitbox> h2);
	//does not work with rotation
	static bool testFixedRectangleCollision(shared_ptr<Hitbox> h1, shared_ptr<Hitbox> h2);
};

