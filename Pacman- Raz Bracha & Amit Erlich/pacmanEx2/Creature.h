#pragma once
#include "Point.h"
#include "io_utils.h"

class Creature 
{
	Point body;
	Directions dir;
public:
	/* C-tor */
	Creature(int x, int y, char ch, Directions direction);

	/* Set and Get Functions */
	void setFigure(char ch);
	void setPosition(const int& x, const int& y);
	void setDirection(const Directions& dir);
	void setColor(const Color& color);
	char getFigure()const;
	Directions getDirection() const;
	Point& getPosition();
	Point getNextPosition() const;

	void restartPosition(Point pos, Directions direction);

	/* Move Ghost Function */
	void move(char ch = ' ');
	void changeDirection();

	int getRandNumFrom0To3();

	void draw();
};