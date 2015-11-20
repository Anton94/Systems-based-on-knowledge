#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>
#include <math.h>
#include <fstream>
#include "constants.h"

inline int nearestInt(float x) { return (int) floor(x + 0.5f); }



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
