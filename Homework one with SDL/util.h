#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>
#include <math.h>
#include <fstream>
#include "constants.h"

inline double signOf(double x) { return x > 0 ? +1 : -1; }
inline double sqr(double a) { return a * a; }
inline double toRadians(double angle) { return angle / 180.0 * PI; }
inline double toDegrees(double angle_rad) { return angle_rad / PI * 180.0; }
inline int nearestInt(float x) { return (int) floor(x + 0.5f); }

/// returns a random floating-point number in [0..1).
/// This is not a very good implementation. A better method is to be employed soon.
inline float randomFloat() { return rand() / (float) RAND_MAX; }



// Sets the number of rows and colums of the matrix given in inputstream
inline void getMatrixBounds(int& width, int& height, std::ifstream& in)
{
	size_t start = in.tellg();
	width = 0;
	height = 0;
	char ch = ' ';
	while (in && ch != '\n')
	{
		in.get(ch);
		if (ch != ',' && ch != '\n')
			++width;
	}

	while (in)
	{
		if (ch == '\n')
			++height;
		in.get(ch);
		if (!in || ch == EOF)
			break;
	}

	in.clear();
	in.seekg(start);
}

#endif // __UTIL_H__
