#include <iostream>
#include "Puzzle.h"


int main()
{
	try
	{
		Puzzle puzzle;
		if (!puzzle.loadMap("map1.csv"))
			throw "Something is wrong with the map file!";
		puzzle.printMap(std::cout);
	}
	catch (const char * msg)
	{
		std::cout << "Error: " << msg << std::endl;
	}
	return 0;
}