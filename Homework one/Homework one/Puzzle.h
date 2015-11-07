#ifndef PUZZLE_H
#define PUZZLE_H

#include <stdio.h>
#include <vector> // the map and priority queue
#include <queue>  // priority queue
#include <string> // for throw formating...
#include "Utility.h"

using namespace std;

class Puzzle
{
	struct Cell
	{
		int x, y;    // Representation
		char symbol; // Representation
		double priceWalkedBlocks;    // Heuristics
		double pricePotentialToFood; // Heuristics
		bool visited; // Search
		Cell* parent; // Search
		Cell(int i = 0, int j = 0, char s = ' ', double wb = 0, double te = 0, Cell * f = NULL, bool v = false) : x(i), y(j), symbol(s), priceWalkedBlocks(wb), pricePotentialToFood(te), parent(f), visited(v) {}
		bool operator>(const Puzzle::Cell& rhs) const { return priceWalkedBlocks + pricePotentialToFood > rhs.priceWalkedBlocks + pricePotentialToFood; }
	};

	class CellComparison // This class is so I can sort my Cells in the priority queue and the smallest price to be on the top of the queue.
	{
	public:
		bool operator() (const Puzzle::Cell* lhs, const Puzzle::Cell* rhs) const
		{
			return *lhs > *rhs;
		}
	};
private:
	vector<vector<Cell>> map;
	int mapWidth;
	int mapHeight;
	Cell * monster;
	Cell * food;
	char WALL;
	char FREE;
	char WATER;
	char MONSTER;
	char FOOD;
	double diagonalCost;
	double directCost;
	double waterCost;
private:
public:
	Puzzle()
	{
		setDefaultValues();
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
				} while (ch == ',' || ch == '\n'); 
				if (ch == EOF || !in)
					return false; // Something went wrong..

				map[i][j].x = j;
				map[i][j].y = i;
				map[i][j].symbol = ch;

				if (ch == MONSTER)
					monster = &map[i][j];
				else if (ch == FOOD)
					food = &map[i][j];
				else if (ch != WALL && ch != FREE && ch != WATER && ch != '\n' && ch != EOF)
					throw string("In the given map there is some invalid symbol '") + ch + string("' at position (") + to_string(i) + string(",") + to_string(j) + string(")!");				
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

	// Solves the puzzle and prints the path to the given ostream. 
	// After this function, I can go from the FOOD cell by it`s parents to get the generated path.
	void solve(std::ostream& out)
	{
		if (!monster || !food)
			throw "Invalid monster or food cells!";

		priority_queue<Cell*, vector<Cell*>, Puzzle::CellComparison> front;
		monster->visited = true;
		front.push(monster);

		Cell * current = NULL;

		while (!front.empty())
		{
			// Let`s get the 'best' node
			current = front.top();
			front.pop();

			// If we found the food, breaks.
			if (current == food)
				break;

			pushCellsChildren(current, front);
		}

		// Reset all cells to be unvisited.
		resetVisitedValuesOfTheCells();
	}

	// Basic print makes the path cells '*'.
	void basicVisualizePath(ostream& out)
	{
		Cell * cell = food;
		if (cell)
			while (cell->parent)
			{
				cell->symbol = '*';
				cell = cell->parent;
			}

		printMap(out);
	}
private:
	// Reset all cell`s visited value to false.
	void resetVisitedValuesOfTheCells()
	{
		for (int i = 0; i < mapHeight; ++i)
			for (int j = 0; j < mapWidth; ++j)
				map[i][j].visited = false;
	}
	// Pushes the children of the given Cell to the given priority queue.
	void pushCellsChildren(Puzzle::Cell * current, priority_queue<Puzzle::Cell*, vector<Puzzle::Cell*>, Puzzle::CellComparison> & front)
	{		
		Cell * child = NULL;
		
		// Lest check all neighbour cells. (NOTE: the case where @i and @j are zero, the current cell is visited, so nothing will happen.)
		for (int i = -1; i <= 1; ++i)
			for (int j = -1; j <= 1; ++j)
			{
				child = getCellAt(current->x + i, current->y + j);
				if (child) // If there is child there(valid cell at this position)
				{
					child->parent = current;
					child->visited = true;
					calculateCellDistanceToFood(child); // this function also set`s the potential distance
					child->priceWalkedBlocks = current->priceWalkedBlocks;
					
					// Let`s calculate the cost of the move to this child.
					if (current->symbol == WATER) // cost to get out from water cell in any direction is fixed - @waterCost
						child->priceWalkedBlocks += waterCost;
					else if (child->x == current->x || child->y == current->y) // If it`s a direct move(left, right, up or down)
						child->priceWalkedBlocks += directCost;
					else // it`s diagonal move
						child->priceWalkedBlocks += diagonalCost;

					// Add the child to the priority queue.
					front.push(child); 
				}
			}
	}

	// Returns a pointer to the cell with the given coords.
	Puzzle::Cell * getCellAt(int x, int y) 
	{
		if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight || // check if the cell is outside of the map
			map[y][x].symbol == WALL || map[y][x].visited) 		 // check if the cell is wall or the cell is already visited
			return NULL;

		return &map[y][x];
	}

	// Calculates AND SETS the distacne to the Food.
	// Goest as much as possible blockes diagonally and other once direct.
	void calculateCellDistanceToFood(Puzzle::Cell* cell)
	{
		// Let`s distance be the distance between cell.x and food.x (cell.y and food.y)
		// The smaller distance goes diagonally and the bigger distance (minus smaller one) goes direct.
		int xDistance = abs(cell->x - food->x);
		int yDistance = abs(cell->y - food->y);

		// The potential price is the diagonal distance + direct distance
		// cell->pricePotentialToFood = smallar_distance * diagonal_cost + (bigger_distance - smallar_distance) * direct_cost
		if (xDistance > yDistance)
			cell->pricePotentialToFood = yDistance * diagonalCost;
		else
			cell->pricePotentialToFood = xDistance * diagonalCost;

		cell->pricePotentialToFood += abs(xDistance - yDistance) * directCost;
	}
public:


	/* 
		Getters for the map symbols.
	*/
	char getWallSymbol() const { return WALL; }
	char getFreeSymbol() const { return FREE; }
	char getWaterSymbol() const { return WATER; }
	char getMonsterSymbol() const { return MONSTER; }
	char getFoodSymbol() const { return FOOD; }
	double getDiagonalCost() const { return diagonalCost; }
	double getDirectCost() const { return directCost; }
	double getWaterCost() const { return waterCost; }

	/*
		Setters for the map symbols.
		NOTE: If you change the symbols after the map is loaded, it can cause invalid calculations!
	*/
	void setWallSymbol(char c) { WALL = c; }
	void setFreeSymbol(char c) { FREE = c; }
	void setWaterSymbol(char c) { WATER = c; }
	void setMonsterSymbol(char c) { MONSTER = c; }
	void setFoodSymbol(char c) { FOOD = c; }
	void setDiagonalCost(double cost) { diagonalCost = cost; }
	void setDirectCost(double cost) { directCost = cost; }
	void setWaterCost(double cost) { waterCost = cost; }

private:
	void setDefaultValues()
	{
		mapWidth = mapHeight = 0;
		monster = food = NULL;
		WALL = 'N';
		FREE = ' ';
		WATER = '~';
		MONSTER = 'M';
		FOOD = 'X';
		diagonalCost = 1.5;
		directCost = 1.0;
		waterCost = 2.0;
	}
};
#endif