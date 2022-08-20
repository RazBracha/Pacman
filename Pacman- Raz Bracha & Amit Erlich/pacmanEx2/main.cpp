#include <windows.h>
#include <iostream>
#include "ThePacmanGame.h"
using namespace std;


int main()
{
	hideCursor();
	ThePacmanGame().run();
	return 0;
}