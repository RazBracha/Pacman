#include "ThePacmanGame.h"
#include "MenuAndMessages.h"

/* ------ ThePacmanGame ------
* This function sets the game.
----------------------------*/
ThePacmanGame::ThePacmanGame()
{
	pacmanEaten = false;
	gamePaused = false;
	moveGhosts = false;
	moveFruit = false;
	isFruitExist = false;
	gameFinished = GameStatus::RUN; // 0 = game not over, 1 = player won, 2 = player lost.
	numOfGhosts = 0;
	count20times = 0;
	count3times = 0;
	timeToCreateFruit = getRandNumFrom0To9();
}

/* -- setGameColors ---
* Sets the game colors.
---------------------*/
void ThePacmanGame::setGameColors()
{
	if (menus.getColorMode()) // if color mode is on
	{
		board.setColor(Color::LIGHTBLUE);
		pacman.setColor(Color::YELLOW);
		fruit.setColor(Color::LIGHTRED);
	}
	else
	{
		board.setColor(Color::WHITE);
		pacman.setColor(Color::WHITE);
		fruit.setColor(Color::WHITE);
	}
}

/* -- setGhostColor --
* Sets ghost color.
--------------------*/
void ThePacmanGame::setGhostColor(int ghostNum)
{
	if (menus.getColorMode())
		ghost[ghostNum]->setColor(ghostColors[ghostNum]);
	else
		ghost[ghostNum]->setColor(Color::WHITE);
}

/* ------------------------------------------- setGame -------------------------------------------
* Checks if board is valid and print error message if it is not (will be completed in Exercise 3).
* If it is ok - reads board from file and sets food, all creatues positions.
* Prints the board and the sideBar data.
------------------------------------------------------------------------------------------------*/
void ThePacmanGame::setGame()
{
	clear_screen();

	if (!board.init(menus.getScreenToLoad())) // if board invalid - show error message
	{
		// not finished yet - will be completed in Exercise 3
		cout << "< board invalid! >";
	}
	/*else
	{*/
		// board is valid
		food.setNumberOfFood(board.getMaxNumOfFood());

		board.printBoard();

		restartPositions();

		pacman.printLivesToScreen(board.getSideBar(), menus.getColorMode());
		pacman.printPointsToScreen(board.getSideBar());

		gameFinished = GameStatus::RUN;
	//}
}

/* ----- getRandNumFrom0To9 ------
* Returns a number between 0 to 9.
--------------------------------*/
int ThePacmanGame::getRandNumFrom0To9()
{
	return (rand() % 10);
}

/* ------------------ getRandPositionForFruit ------------------
* Return: pointToReturn
* 
* Creates a random point within the board frame,
* and checks if its position is valid.
* Repeat these steps until the position is valid and returns is.
--------------------------------------------------------------*/
Point ThePacmanGame::getRandPositionForFruit()
{
	int x, y, width = board.getWidth(), length = board.getLength();
	bool validPos = false;
	Point pointToReturn, sideBar = board.getSideBar();

	while (!validPos)
	{
		// create random point inside the board
		x = (rand() % (length - 2)) + 1;
		y = (rand() % (width - 2)) + 1;
		pointToReturn.init(x, y);

		if (board.getCharInBoard(pointToReturn) != Characters::WALL) // check if there is a wall in this position
		{
			// check if position is inside sideBar
			if (y == (sideBar.getY() + 1))
			{
				if ((x < sideBar.getX()) || (x > (sideBar.getX() + 19)))
					validPos = true; // if not inside
			}
			else
				validPos = true; // if not a wall
		}
	}
	return pointToReturn;
}

/* -- resetGame ---
* Reset a new game.
-----------------*/
void ThePacmanGame::resetGame()
{
	setGameColors();
	pacman.restartLifeAndPoints();

	setGame();
}

/* ----------------------------- resetFruit -----------------------------
* Resets all fruit-releted variables and delete fruit figure from screen.
-----------------------------------------------------------------------*/
void ThePacmanGame::resetFruit()
{
	// clear fruit position from screen
	char tempCh = board.getCharInBoard(fruit.getPosition());
	(tempCh == Characters::EMPTY_PLACE) ? (clear(fruit.getPosition())) : (fruit.getPosition().draw(board.getCharInBoard(fruit.getPosition())));

	count3times = 0;
	timeToCreateFruit = getRandNumFrom0To9();
	isFruitExist = false;
}

/* ------------ restartPositions ------------
* Sets all creaturs to their start positions.
-------------------------------------------*/
void ThePacmanGame::restartPositions()
{
	deleteAllGhosts();

	Point* creatures = board.getCreaturesArr();
	numOfGhosts = board.getNumOfGhost();
	pacman.restartPosition(creatures[0], Directions::STAY);

	for (int i = 0; i < numOfGhosts; i++)
	{
		switch (menus.getGameLevel())
		{
		case GameLevel::BEST:
			ghost[i] = new GhostBest();
			dynamic_cast<GhostBest*>(ghost[i])->setCount5steps(i);
			break;
		case GameLevel::GOOD:
			ghost[i] = new GhostNovice();
			break;
		case GameLevel::NOVICE:
			ghost[i] = new GhostNovice();
			break;
		default:
			break;
		}
		setGhostColor(i);
		ghost[i]->restartPosition(creatures[i + 1], Directions::UP);
	}

	resetFruit();
}

/* -------------- clearPositions --------------
* Clears pacman and ghosts positions from sceen
---------------------------------------------*/
void ThePacmanGame::clearPositions()
{
	clear(pacman.getPosition());

	for (int i = 0; i < numOfGhosts; i++)
		clear(ghost[i]->getPosition());
}

/* ------------------------- run --------------------------
* Prints the board and in charge of the course of the game.
---------------------------------------------------------*/
void ThePacmanGame::run()
{
	do
	{
		menus.resetWantToLeave();
		menus.menuWindow();
		if (!menus.getWantToLeave())
		{
			resetGame();

			do // runs as long as player did not lose and there is at least one screen
			{
				char key = 0;
				int dir;
				do
				{
					if (_kbhit()) // if user pressed any key
					{
						key = _getch();
						if (key == ESC) // pause / unpause the game
							gamePaused = !gamePaused;
						else if ((key == EXIT) || (key == EXIT + 32)) // user choose to exit the game (EXIT = G, EXIT + 32 = 'g')
						{
							gameFinished = GameStatus::GET_OUT;
							menus.resetScreenWasSelected();
						}
						else if ((dir = pacman.getDirection(key)) != -1) // change pacman direction
							pacman.setDirection((Directions)dir);
					}
					if (!gameFinished && !gamePaused) // if game not paused
					{
						handlePacman();
						
						if (numOfGhosts && !pacmanEaten && moveGhosts) // (if ghost exists) ghosts can move every 2 pacman's steps
							handleGhosts();

						if (count3times == 3) // fruit can move every 3 pacman's steps
							handleFruit();
						else
							count3times++;

						moveGhosts = !moveGhosts; // ghost move switch
						Sleep(150);
					}

				} while (gameFinished == GameStatus::RUN);

				if (menus.isThereNextScreen())
					if (gameFinished == GameStatus::WON) // if player has won the current screen, go to the next game screen
						setGame();

			} while (gameFinished == GameStatus::RUN); // exits when: player chose to leave / he lost / he finished all the screens
			updateHighScore();
			gameFinishedSituation();
		}
	} while (!menus.getWantToLeave()); // player chose to exit the program
	deleteAllGhosts();
}

/* ------------------------- eatFood --------------------------
* Get: position
* 
* Deletes food from board in the position the function recived,
* and update pacman points.
-------------------------------------------------------------*/
void ThePacmanGame::eatFood(Point position)
{
	board.clearCharInBoard(position); // delete food from board (array)
	pacman.updatePoints(Foods::FOOD_POINTS, board.getSideBar()); // add point to pacman
	food.decreasNumberOfFood();
	if (food.getNumberOfFood() == 0) // if pacman ate all food
			gameFinished = GameStatus::WON;
}

/* ------------------------- swapGhosts --------------------------
* Swap between GhostBest and GhostNovice,
* (be GhostNovice for 5 moves and than be GhostBest for 15 moves).
----------------------------------------------------------------*/
void ThePacmanGame::swapGhosts()
{
	if (count20times == 5) // change to GhostBest
	{
		for (int i = 0; i < numOfGhosts; i++)
		{
			Ghost* tempGhost = ghost[i]; // save the current pointer to the ghost
			ghost[i] = new GhostBest();
			dynamic_cast<GhostBest*>(ghost[i])->setCount5steps(i);
			ghost[i]->restartPosition(tempGhost->getPosition(), Directions::UP); // copy position from old ghost to new
			delete tempGhost; // delete old ghost pointer
			setGhostColor(i);
		}
	}
	else if (count20times == 20) // change to GhostNovice
	{
		for (int i = 0; i < numOfGhosts; i++)
		{
			Ghost* tempGhost = ghost[i]; // save the current pointer to the ghost
			ghost[i] = new GhostNovice();
			ghost[i]->restartPosition(tempGhost->getPosition(), Directions::UP); // copy position from old ghost to new
			delete tempGhost; // delete old ghost pointer
			setGhostColor(i);
		}
		count20times = 0;
	}
	count20times++;
}

/* ------------- deleteAllGhosts -------------
* Deletes all ghosts pointer from ghost-array.
--------------------------------------------*/
void ThePacmanGame::deleteAllGhosts()
{
	if (ghost[0] != nullptr) // if there are ghosts - delete their pointer
		for (int i = 0; i < numOfGhosts; i++)
			delete ghost[i];
}

/* ---------------------------- updateHighScore ----------------------------
* Check if current pacman's score is higher than the highest score recorded,
* and updates highScore file if it is.
--------------------------------------------------------------------------*/
void ThePacmanGame::updateHighScore()
{
	int currHighestScore = menus.getHighestScore();

	if (pacman.getPoints() > currHighestScore)
	{
		fstream scoreFile("highScore.txt", ios::out | ios::trunc);
		if (!scoreFile) // if failed to open
			cout << "File could not be open.";
		else
			scoreFile << pacman.getPoints(); // insert the new high score to highScore file
	}
}

/* ----------- checkIfPacmanCanMove -----------
* Return: false if cant move, otherwise - true.
---------------------------------------------*/
bool ThePacmanGame::checkIfPacmanCanMove()
{
	Point nextPosition = pacman.getNextPosition();
	char nextPositionChar = board.getCharInBoard(nextPosition);

	// check if pacman hit ghost
	checkIfGhostAndPacmanHit(nextPosition);
	if (pacmanEaten) // pacman hit ghost
		return false;
	else if (nextPositionChar == Characters::WALL) // there is a wall
		return false;
	else if (nextPositionChar == Characters::BREADCRUMBS) // there is food
		eatFood(nextPosition);
	return true;
}

/* ------------ checkIfGhostCanMove ----------- 
* Return: false if cant move, otherwise - true.
---------------------------------------------*/
bool ThePacmanGame::checkIfGhostCanMove(int ghostNum)
{
	ghost[ghostNum]->changeDirection();
	Point nextPosition = ghost[ghostNum]->getPosition();
	nextPosition.move(ghost[ghostNum]->getDirection());
	char nextPositionChar = board.getCharInBoard(nextPosition);

	// there is a wall or invisible tunnel
	if ((nextPositionChar == Characters::WALL) || (nextPositionChar == Characters::INVISIBLE_TUNNEL)) 
		return false;

	checkIfGhostAndPacmanHit(pacman.getPosition());
	if (pacmanEaten) // ghost hit pacman
		return false;
	return true;
}

/* ----------- checkIfFruitCanMove ------------
* Return: false if cant move, otherwise - true.
---------------------------------------------*/
bool ThePacmanGame::checkIfFruitCanMove()
{
	fruit.changeDirection();
	Point nextPosition = fruit.getPosition();
	nextPosition.move(fruit.getDirection());
	char nextPositionChar = board.getCharInBoard(nextPosition);

	// there is a wall or invisible tunnel
	if ((nextPositionChar == Characters::WALL) || (nextPositionChar == Characters::INVISIBLE_TUNNEL)) 
		return false;

	return true;
}

/* -------------- checkIfGhostAndPacmanHit --------------- */
void ThePacmanGame::checkIfGhostAndPacmanHit(Point pacmanPos)
{
	// compare pacman and ghosts positions
	for (int i = 0; i < numOfGhosts; i++)
	{
		if (compare(pacmanPos, ghost[i]->getPosition()))
		{
			pacmanEaten = true;
			break;
		}
	}
}

/* ------- checkIfPacmanHitFruit ------- */
void ThePacmanGame::checkIfPacmanHitFruit()
{
	if (compare(pacman.getPosition(), fruit.getPosition()))
	{
		resetFruit();
		pacman.updatePoints(fruit.getPoints(), board.getSideBar()); // add point to pacman
		pacman.draw();
	}
}

/* ------------- checkIfGhostHitFruit ------------- */
void ThePacmanGame::checkIfGhostHitFruit(int ghostNum)
{
	if (compare(ghost[ghostNum]->getPosition(), fruit.getPosition()))
	{
		resetFruit();
		ghost[ghostNum]->draw();
	}
}

/* ------- handlePacman ------- */
void ThePacmanGame::handlePacman()
{
	if (checkIfPacmanCanMove())
	{
		pacman.move();
		if (isFruitExist)
			checkIfPacmanHitFruit();
	}
	else if (pacmanEaten)
		handlePacmanEatenSituation();
}

/* ------- handleGhosts ------- */
void ThePacmanGame::handleGhosts()
{
	if (menus.getGameLevel() == GameLevel::GOOD)
		swapGhosts();

	for (int i = 0; i < numOfGhosts; i++) // check ghost movement for every ghost
	{
		if (checkIfGhostCanMove(i))
		{
			// draw the right char in ghost position before she moves
			char tempCh = board.getCharInBoard(ghost[i]->getPosition());
			if (tempCh == Characters::EMPTY_PLACE)
				ghost[i]->moveGhost(' ', pacman.getPosition().getX(), pacman.getPosition().getY());
			else
				ghost[i]->moveGhost(tempCh, pacman.getPosition().getX(), pacman.getPosition().getY());

			if (isFruitExist)
				checkIfGhostHitFruit(i);
		}
		else
			ghost[i]->draw();
	}

	if (pacmanEaten)
		handlePacmanEatenSituation();
}

/* ------- handleFruit ------- */
void ThePacmanGame::handleFruit()
{
	count3times = 0;
	if ((timeToCreateFruit == 0) && (!isFruitExist))
	{
		fruit.reset(getRandPositionForFruit());
		isFruitExist = true;
	}
	else if (timeToCreateFruit == 0) // fruit exists
	{
		fruit.decreasNumberOfSteps();
		if (fruit.getNumberOfSteps() == 0) // time for fruit to disappear
			resetFruit();
		else
		{
			if (checkIfFruitCanMove())
			{
				char tempCh = board.getCharInBoard(fruit.getPosition());
				(tempCh == Characters::EMPTY_PLACE) ? (fruit.move()) : (fruit.move(tempCh));

				// check if fruit hit any creature
				checkIfPacmanHitFruit();
				for (int i = 0; i < numOfGhosts; i++)
					checkIfGhostHitFruit(i);
			}
		}
	}
	else
		timeToCreateFruit--;
}

/* ------- handlePacmanEatenSituation ------- */
void ThePacmanGame::handlePacmanEatenSituation()
{
	pacman.updateLives(board.getSideBar(), menus.getColorMode()); // pacman loses one life
	if (pacman.getLife() == 0) // if lost all lives
		gameFinished = GameStatus::LOST;
	else
	{
		clearPositions();
		restartPositions();
	}
	pacmanEaten = false;
}

/* ----- gameFinishedSituation --------
* Prints victory / lose / exit message.
-------------------------------------*/
void ThePacmanGame::gameFinishedSituation()
{
	if (gameFinished == GameStatus::WON)
		menus.printVictoryMsg();
	else if (gameFinished == GameStatus::LOST)
		menus.printLoseMsg();
	else
		menus.printExitGameMsg();
	_getch(); // wait until any key is pressed
}