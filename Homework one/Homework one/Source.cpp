#include <iostream>
#include "Puzzle.h"

int main()
{
	try
	{
		Puzzle puzzle;
		if (!puzzle.loadMap("map2.csv"))
			throw "Something is wrong with the map file!";
		puzzle.printMap(std::cout);
		puzzle.solve(std::cout);
		puzzle.basicVisualizePath(std::cout);
	}
	catch (const char * msg)
	{
		std::cout << "Error: " << msg << std::endl;
	}
	catch (const string msg)
	{
		std::cout << "Error: " << msg << std::endl;
	}

	return 0;
}