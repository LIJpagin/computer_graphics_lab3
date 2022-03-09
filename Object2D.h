#pragma once
#include <windows.h>
#include <vector>
#include <queue>
#include <math.h>
#ifndef OBJECT2D_H
#define OBJECT2D_H

class Object2D {
private:
	struct Coord2D { double x, y; };
	std::vector <Coord2D> points_ = { {0, 0} };
	unsigned int action_point_ = 0;
	unsigned int object_number_;
	static unsigned int action_object_;
	static unsigned int number_objects_;
	int width_pen_ = 3;
	COLORREF color_pen_ = 0x000000;



protected:
	void lineBresenham(HDC, int, int, int, int, COLORREF) const;
	void drawPoint(HDC) const;
	void highlightActiveObject(HDC) const;



public:
	Object2D() { object_number_ = number_objects_++; }
	Object2D(double, ...);
	void points(double, ...);
	void addPoint(double, double);

	void moving(double, double);
	void scalingByAxes(double, double);
	void scaling(double Sxy) { scalingByAxes(Sxy, Sxy); }
	void rotation(double);

	void installPen(COLORREF, int);
	void drawOpenContour(HDC) const;
	void drawClosedContour(HDC) const;
	void drawBresenham(HDC) const;
	void drawPaintedContour(HDC) const;

	void changePoint() { action_point_ == points_.size() - 1 ? action_point_ = 0 : action_point_++; }
	static void changeObject() { action_object_ == number_objects_ - 1 ? action_object_ = 0 : action_object_++; }
	static unsigned int numberObjects() { return number_objects_; }
	static unsigned int activeObject() { return action_object_; }
	void regularPolygon(int, double, double, double);

	~Object2D() { points_.clear(); }
};

#endif OBJECT2D_H