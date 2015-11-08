#include <SDL/SDL.h>
#include "sdl.h"
#include "puzzle.h"

Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE]; //!< virtual framebuffer

void renderScene(void)
{
	for (int y = 0; y < frameHeight(); y++)
		for (int x = 0; x < frameWidth(); x++)
			vfb[y][x] = Color(x / (float) frameWidth(), y / (float) frameHeight(), 0.0f);
}

int main(int argc, char** argv)
{
	if (!initGraphics(RESX, RESY)) return -1;
	renderScene();
	displayVFB(vfb);
	waitForUserExit();
	closeGraphics();
	return 0;
}
