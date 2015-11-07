#ifndef PUZZLE_H
#define PUZZLE_H

#include <stdio.h>
#include <vector>
#include "Utility.h"

using namespace std;

class Puzzle
{
	struct Cell
	{
		int x, y;
		char symbol;
		int price; // Evristikata
		Cell* parent;
		Cell(int i = 0, int j = 0, char s = ' ', int p = 0, Cell * f = NULL) : x(i), y(j), symbol(s), price(p), parent(f) {}
	};

private:
	vector<vector<Cell>> map;
	int mapWidth;
	int mapHeight;
public:
	Puzzle()
	{
		mapWidth = mapHeight = 0;
	}
	// Loads the map from csv file. If the file is not valud returns false.
	bool loadMap(const char * fileName)
	{
		std::ifstream in(fileName);
		if (!in)
			return false;

		char ch;
		getMatrixBounds(mapWidth, mapHeight, in);

		map.resize(mapHeight);
		for (int i = 0; i < mapHeight; ++i)
		{
			map[i].resize(mapWidth);
			
			for (int j = 0; j < mapWidth; ++j)
			{
				do
				{
					in.get(ch);
					if (!in)
						return false;
				} while (ch == ',' || ch == '\n'); // I can Add more stuff, like to miss other fucked up symbols or something...
				if (ch == EOF || !in)
					return false; // Something went wrong..

				map[i][j].x = i;
				map[i][j].y = j;
				map[i][j].symbol = ch;
			}
		}

		in.clear();
		in.close();
		return true;
	}

	void printMap(std::ostream& out) const
	{
		out << "Map:\n";
		for (int i = 0; i < mapHeight; ++i)
		{
			out << "[";
			for (int j = 0; j < mapWidth; ++j)
				out << map[i][j].symbol;
			out << "]\n";
		}

	}
};


#endif