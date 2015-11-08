#ifndef __SDL_H__
#define __SDL_H__

#include "color.h"
#include "constants.h"

bool initGraphics(int frameWidth, int frameHeight);
void closeGraphics(void);
void displayVFB(Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE]); //!< displays the VFB (Virtual framebuffer) to the real one.
void waitForUserExit(void); //!< Pause. Wait until the user closes the application
int frameWidth(void); //!< returns the frame width (pixels)
int frameHeight(void); //!< returns the frame height (pixels)

#endif // __SDL_H__
