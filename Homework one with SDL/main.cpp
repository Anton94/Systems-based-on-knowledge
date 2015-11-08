#include <iostream>
#include <SDL/SDL.h>
#include "sdl.h"

Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE]; //!< virtual framebuffer

#include "puzzle.h"

void renderScene(void)
{
	for (int y = 0; y < frameHeight(); y++)
		for (int x = 0; x < frameWidth(); x++)
			//vfb[y][x] = Color(x / (float) frameWidth(), y / (float) frameHeight(), 0.0f;
			vfb[y][x] = Color(1,1,1);
}
void renderScene2(void)
{
	for (int y = 0; y < frameHeight(); y++)
		for (int x = 0; x < frameWidth(); x++)
			//vfb[y][x] = Color(x / (float) frameWidth(), y / (float) frameHeight(), 0.0f;
			vfb[y][x] = Color(0,1,1);
}

int main(int argc, char** argv)
{
    freopen("CON", "w", stdout); // redirects stdout because SDL redirects it to a file.


	if (!initGraphics(RESX, RESY)) return -1;
	renderScene();
	displayVFB(vfb);
    try
	{
		Puzzle puzzle;
		if (!puzzle.loadMap("map3.csv"))
			throw "Something is wrong with the map file!";
		puzzle.printMap(std::cout);
		puzzle.solve(std::cout);
		//puzzle.basicVisualizePath(std::cout);
		puzzle.printFormatedPath(std::cout);
		puzzle.solveAndVizualize(std::cout);
	}
	catch (const char * msg)
	{
		std::cout << "Error: " << msg << std::endl;
	}
	catch (const string msg)
	{
		std::cout << "Error: " << msg << std::endl;
	}
	waitForUserExit();
	closeGraphics();
	return 0;
}
