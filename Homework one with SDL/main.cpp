/*
*
*    Name: Anton Vasilev Dudov
*    # 71488
*
*    Github repository: https://github.com/Anton94/Systems-based-on-knowledge/tree/master/Homework%20one%20with%20SDL
*
*    A star path finding.
*
*    Command line arguments:
*       1) file name for the map
*       2) X coord for monster
*       3) Y coord for monster
*       4) X coord for food
*       5) Y coord for food
*    And optional two:
*       6) Flag for visualization of child pushing in the queue. By default it`s true
*       7) Delay in milliseconds for the frames. By default it`s 0
*/



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
int fromStringToInt(char * str)
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

        puzzle.setMonsterAndFoodCoords(fromStringToInt(argv[2]), fromStringToInt(argv[3]), fromStringToInt(argv[4]), fromStringToInt(argv[5]));

        // The flag for the SDL visualization
        if (argc >= 7)
        {
            puzzle.setVisualizationFlag(fromStringToInt(argv[6]));
        }

        // The flag for the SDL visualization
        if (argc >= 8)
        {
            puzzle.setDelay(fromStringToInt(argv[7]));
        }
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
