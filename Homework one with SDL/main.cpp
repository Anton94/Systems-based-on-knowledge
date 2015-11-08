#include <iostream>
#include <SDL/SDL.h>
#include "sdl.h"
#include "puzzle.h"

Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE]; //!< virtual framebuffer

void renderScene(void)
{
	for (int y = 0; y < frameHeight(); y++)
		for (int x = 0; x < frameWidth(); x++)
			vfb[y][x] = Color(x / (float) frameWidth(), y / (float) frameHeight(), 0.0f);
			//vfb[y][x] = Color(255,255,0);
}

int main(int argc, char** argv)
{
    freopen("CON", "w", stdout); // redirects stdout because SDL redirects it to a file.

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

	if (!initGraphics(RESX, RESY)) return -1;
	renderScene();
	displayVFB(vfb);
	waitForUserExit();
	closeGraphics();
	return 0;
}
