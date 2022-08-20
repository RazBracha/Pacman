#pragma once
#include "io_utils.h"
#include "MenuAndMessages.h"
#include "Board.h"
#include "Food.h"
#include "Pacman.h"
#include "Point.h"
#include "Ghost.h"
#include "GhostBest.h"
#include "GhostNovice.h"
#include "Fruit.h"

class ThePacmanGame {
	enum { ESC = 27, EXIT = 71 };
	enum { MAX_NUM_OF_GHOSTS = 4 };
	enum GameStatus { RUN = 0, WON = 1, LOST = 2, GET_OUT = 3 };
	enum Foods { FOOD_POINTS = 1 };

	MenuAndMessages menus;

	Board board;
	Food food;
	Pacman pacman;
	Ghost* ghost[MAX_NUM_OF_GHOSTS];
	Fruit fruit;

	bool pacmanEaten; 
	bool gamePaused;
	bool moveGhosts;
	bool moveFruit;
	bool isFruitExist;
	int gameFinished;

	int numOfGhosts;
	int count20times; // for the ghost use
	int count3times; // for the fruit use
	int timeToCreateFruit;

	Color ghostColors[MAX_NUM_OF_GHOSTS] = { Color::LIGHTCYAN , Color::LIGHTMAGENTA , Color::BROWN, Color::LIGHTGREEN };

public:
	// C-tor
	ThePacmanGame();

	// Set and Get Functions
	void setGameColors();
	void setGhostColor(int ghostNum);
	void setGame();

	int getRandNumFrom0To9();
	Point getRandPositionForFruit();

	// Restart Functions
	void resetGame();
	void resetFruit();
	void restartPositions();
	void clearPositions();

	void run();

	void eatFood(Point position);
	void swapGhosts();
	void deleteAllGhosts();
	void updateHighScore();

	// Check Functions
	bool checkIfPacmanCanMove();
	bool checkIfGhostCanMove(int ghostNum);
	bool checkIfFruitCanMove();
	void checkIfGhostAndPacmanHit(Point pacmanPos);
	void checkIfPacmanHitFruit();
	void checkIfGhostHitFruit(int ghostNum);
	
	// Handle situation Functions
	void handlePacman();
	void handleGhosts();
	void handleFruit();
	void handlePacmanEatenSituation(); 
	void gameFinishedSituation();
};