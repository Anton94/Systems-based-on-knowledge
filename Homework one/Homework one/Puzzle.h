#ifndef PUZZLE_H
#define PUZZLE_H

#include <stdio.h>
#include <vector>
#include <queue>
#include "Utility.h"

using namespace std;


enum CellType
{
	WALL = '#',
	FREE = ' ',
	WATER = '~',
	MONSTER = 'M',
	FOOD = 'X'
};

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
	Cell * monster;
	Cell * food;
public:
	Puzzle()
	{
		mapWidth = mapHeight = 0;
		monster = food = NULL;
	}

	// Sets the monster and the food cells. If the values are invalid- throws exeption.
	void setMonsterAndFoodCoords(int monsterX, int monsterY, int foodX, int foodY)
	{
		if (monsterX >= mapWidth || monsterY >= mapHeight || monsterX < 0 || monsterY < 0)
			throw "Invalid monster coordinates for the given map!";
		if (foodX >= mapWidth || foodY >= mapHeight || foodX < 0 || foodY < 0)
			throw "Invalid food coordinates for the given map!";

		monster = &map[monsterX][monsterY];
		food = &map[foodX][foodY];

		if (monster->symbol != MONSTER)
			throw "Invalid value in the monster cell, there`s not a monster there!";

		if (food->symbol != FOOD)
			throw "Invalid value in the food cell, there`s not any food there!"; 
	}

	// Loads the map from csv file. If the file is not valud returns false. 
	// Auto-allocates the position for the monster and the food.
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

				if (ch == MONSTER)
					monster = &map[i][j];
				if (ch == FOOD)
					food = &map[i][j];
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

	// Solves the puzzle and prints the path to the given ostream
	void solve(std::ostream& out)
	{
		if (!monster || !food)
			throw "Invalid monster or food cells!";

		priority_queue<Cell*> front;
		front.push(monster);

	}
};


#endif