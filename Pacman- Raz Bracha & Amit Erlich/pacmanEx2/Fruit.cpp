#include "Fruit.h"

/*------------------- Fruit ------------------
* Initialize Fruit with defult values.
--------------------------------------------*/
Fruit::Fruit(int x, int y):Creature(x, y, '5', Directions::UP), stepsLeft(10) {}

/*------------------- getNumberOfSteps --------------------------
* RETURN: how much steps left to the fruit until it disappears
-----------------------------------------------------------------*/
int Fruit::getNumberOfSteps() const
{
	return stepsLeft;
}

/*------------------- getPoints -------------------
* RETURN: how much points the fruit is worth.
---------------------------------------------------*/
int Fruit::getPoints()
{
	return (getFigure() - 48); //from char to int
}

/*------------------- reset -------------------
* reset fruit stepsLeft,figure position.
----------------------------------------------*/
void Fruit::reset(const Point& position)
{
	stepsLeft = 10;
	// random x, y position
	setFigure(randomNumBetween5to9());
	setDirection((Directions)getRandNumFrom0To3());
	setPosition(position.getX(), position.getY());
}

/*---------- randomNumBetween5to9 ------------*/
char Fruit::randomNumBetween5to9()
{
	return (char)((rand() % 5) + 5 + 48); // (char)48 = '0'
}

/*---------- decreasNumberOfSteps ------------*/
void Fruit::decreasNumberOfSteps()
{
	stepsLeft--;
}