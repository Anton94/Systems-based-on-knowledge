#include <iostream>
#include <SDL/SDL.h>
#include "sdl.h"

Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE]; //!< virtual framebuffer

#include "puzzle.h"
#include "util.h"

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

// Makes a unsigned number from given string
int toIntCoord(char * str)
{
    int num = 0;
    while (*str != '\0')
    {
        num = num * 10 + *(str++) - '0';
    }

    return num;
}

int main(int argc, char** argv)
{
    freopen("CON", "w", stdout); // redirects stdout because SDL redirects it to a file.

    /* Parsing the input parameters. */
    if (argc < 6)
    {
        std::cout << "Missing some input parametr. Needed at least 5 but got only " << argc - 1 << std::endl;
        return -1;
    }




	if (!initGraphics(RESX, RESY)) return -1;
	renderScene();
	displayVFB(vfb);

    try
	{
		Puzzle puzzle;
		if (!puzzle.loadMap(argv[1]))
			throw "Something is wrong with the map file!";

        //std::cout <<toIntCoord(argv[2]) << " " << argv[3] << " " << argv[4] << " " << argv[5] << std::endl;
        puzzle.setMonsterAndFoodCoords(toIntCoord(argv[2]), toIntCoord(argv[3]), toIntCoord(argv[4]), toIntCoord(argv[5]));

		puzzle.printMap(std::cout);
		puzzle.solveAndVizualize(std::cout);
		puzzle.visualizeThePath();
		puzzle.basicVisualizePath(std::cout);
		puzzle.printFormatedPath(std::cout);

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
