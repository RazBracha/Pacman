#pragma once
#include <iostream>
#include "Creature.h"

using namespace std;

class Pacman: public Creature
{
	enum Keys { UP_KEY = 'W', DOWN_KEY = 'X', LEFT_KEY = 'A', RIGHT_KEY = 'D', STAY_KEY = 'S' };

	int life;
	int points;
	char arrowKeys[5];
public:
	/* C-tor */
	Pacman(int x = 1, int y = 1, int life = 3, int points = 0);

	/* Set and Get Functions */
	void setArrowKeys();
	int getLife() const;
	int getPoints() const;
	int getDirection(char key) const;

	void restartLifeAndPoints();

	/* Update Functions */
	void updateLives(const Point& position, const bool& colorMode);
	void updatePoints(const int& add, const Point& position);

	/* Print Functions */
	void printLivesToScreen(const Point& position, const bool& colorMode) const;
	void printPointsToScreen(const Point& position) const;
};