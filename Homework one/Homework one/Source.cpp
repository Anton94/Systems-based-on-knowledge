#include <iostream>
#include "Puzzle.h"


int main()
{
	Puzzle puzzle;
	puzzle.loadMap("map1.csv");
	puzzle.printMap(std::cout);
	return 0;
}