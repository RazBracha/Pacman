#pragma once
#include "Creature.h"

class Fruit : public Creature
{
	int stepsLeft;
public:
	Fruit(int x = 1, int y = 1);

	/* Get and Reset Functions */
	int getNumberOfSteps() const;
	int getPoints();
	void reset(const Point& position);

	char randomNumBetween5to9();
	void decreasNumberOfSteps();
};