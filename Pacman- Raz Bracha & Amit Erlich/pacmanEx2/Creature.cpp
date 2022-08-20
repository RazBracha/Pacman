#include "Creature.h"

/* -------------------------------- Creature ----------------------------------
* Get: x, y, direction.
*
* Initialized the creature with default values (or with the values it received).
---------------------------------------------------------------------------*/
Creature::Creature(int x, int y, char ch, Directions direction)
{
	body.init(x, y, ch);
	dir = direction;
}

/* -------------------------------- setFigure ----------------------------------*/
void Creature::setFigure(char ch)
{
	body.setFigure(ch);
}

/* ---- setPosition -----
* Get: x, y
* Set the creature position.
-----------------------*/
void Creature::setPosition(const int& x, const int& y)
{
	body.setX(x);
	body.setY(y);
	body.drawPoint();
}

/* ---- setDirection -----
* Get: dir
* Set the creature direction.
------------------------*/
void Creature::setDirection(const Directions& dir)
{
	this->dir = dir;
}

/* ---- setColor -----
* Get: color
* Set the creature color.
--------------------*/
void Creature::setColor(const Color& color)
{
	body.setColor(color);
}

/* ---- getFigure --------------
* RETURN: creature Figure.
-------------------------------*/
char Creature::getFigure()const
{
	return body.getFigure();
}

/* --------- getDirection ---------
* Return: dirction
----------------------------------*/
Directions Creature::getDirection() const
{
	return dir;
}

/* -- getPosition --
* Return: body
------------------*/
Point& Creature::getPosition()
{
	return body;
}

/* ------------------ getNextPosition ------------------
* Return: p
*
* Returns the next position that the creature will move to.
------------------------------------------------------*/
Point Creature::getNextPosition() const
{
	Point p = body;
	p.move(dir);
	return p;
}

/* -------------- restartPosition -----------------------------
* Sets the creature position (and draw) to its start position.
---------------------------------------------------------------*/
void Creature::restartPosition(Point pos, Directions direction)
{
	body.setX(pos.getX());
	body.setY(pos.getY());
	dir = direction;
	body.drawPoint();
}

/* ------------------------------- move --------------------------------
* Delete the creature char from the screen in its current position,
* moves it according to its direction and redraw it in its new position.
----------------------------------------------------------------------*/
void Creature::move(char ch)
{
	body.draw(ch);
	body.move(dir);
	body.drawPoint();
}

/* ------------------ changeDirection ------------------
* Set creature direction randomly 25% of cases.
------------------------------------------------------*/
void Creature::changeDirection()
{
	// 25% of cases we will change dir
	if (rand() % 4 == 0)
		setDirection((Directions)getRandNumFrom0To3());
}

/* --------- getRandNumFrom0To3 ----------*/
int Creature::getRandNumFrom0To3()
{
	return (rand() % 4);
}

/* ---------- draw ----------*/
void Creature::draw()
{
	body.drawPoint();
}