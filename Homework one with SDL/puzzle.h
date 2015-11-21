#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <windows.h> //Sleep
#include <vector> // the map and priority queue
#include <queue>  // priority queue
#include <string> // for throw formating...
#include "util.h" // SDL stuff
#include "color.h" // SDL stuff
#include "constants.h" // SDL stuff

using namespace std;

class Puzzle
{
	struct Cell
	{
		int x, y;    // Representation
		char symbol; // Representation
		double priceWalkedBlocks;
		double pricePotentialToFood; // Heuristics
		bool visited; // Search
		Color color;  // SDL
		Cell* parent; // Search
		Cell(int i = 0, int j = 0, char s = ' ', double wb = 0, double te = 0, Cell * f = NULL, bool v = false, Color c = Color()) : x(i), y(j), symbol(s), priceWalkedBlocks(wb), pricePotentialToFood(te), parent(f), visited(v), color(c) {}
		bool operator>(const Puzzle::Cell& rhs) const { return priceWalkedBlocks + pricePotentialToFood > rhs.priceWalkedBlocks + rhs.pricePotentialToFood; }
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
	vector< vector<Cell> > map;
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
	Color wallColor;
	Color freeColor;
	Color waterColor;
	Color monsterColor;
	Color foodColor;
    int vfbCellWidth;  // The representation of cell in the SDL map
    int vfbCellHeight; // The representation of cell in the SDL map
    int delay; // Delay between child search

public:
	Puzzle();

	// Sets the monster and the food cells. If the values are invalid- throws exeption.
	void setMonsterAndFoodCoords(int monsterX, int monsterY, int foodX, int foodY);

	// Loads the map from csv file. If the file is not valud returns false.
	// Auto-allocates the position for the monster and the food.
	bool loadMap(const char * fileName);

    // Prints the map on the given ostream.
	void printMap(std::ostream& out) const;

	// Solves the puzzle and sets parents of the cells.
	// After this function, I can go from the FOOD cell by it`s parents to get the generated path.
	void solve(std::ostream& out);

	// Makes the path cells '*'.
	// Prints the map and after that prints the formated path.
	void basicVisualizePath(ostream& out);

	// Prints the path from the monster to his food.
	void printFormatedPath(ostream& out) const;

    // SDL visualization with path finding
	void solveAndVizualize(ostream& out);

    // Visualize the path from the FOOD cell by it`s parents to the MONSTER.(Makes the color of the cells brawn)
	void visualizeThePath();

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
    // Sets the default values of the puzzle members.
	void setDefaultValues();

    // Goes through every cell and makes needed color to the virtual frame buffer
    void fillInitialColors();

	// Prints the path from the monster to his food.
	void printFormatedPath(Cell * cell, ostream& out) const;

    // Visualize the closest path from the Monster to the cell
	void visualizeThePath(Cell * cell);

    // Fills the virtual frame buffer cell with the given Cells color
    void fillVFBCell(Cell * cell);

	// Reset all cell`s visited and parents values.
	void resetValuesOfTheCells();

	// Pushes the children of the given Cell to the given priority queue.
	void pushCellsChildren(Puzzle::Cell * current, priority_queue<Puzzle::Cell*, vector<Puzzle::Cell*>, Puzzle::CellComparison> & front, ostream& out);

	// Returns a pointer to the cell with the given coords.
	Puzzle::Cell * getCellAt(int x, int y);

	// Calculates AND SETS the distacne to the Food.
	// Goest as much as possible blockes diagonally and other once direct.
	void calculateCellDistanceToFood(Puzzle::Cell* cell);
};

inline Puzzle::Puzzle()
{
    setDefaultValues();
}

// Sets the monster and the food cells. If the values are invalid- throws exeption.
inline void Puzzle::setMonsterAndFoodCoords(int monsterX, int monsterY, int foodX, int foodY)
{
    std::cout << monsterX << " " << monsterY << " " << foodX << " " << foodY << std::endl;
    if (monsterX >= mapWidth || monsterY >= mapHeight || monsterX < 0 || monsterY < 0)
        throw "Invalid monster coordinates for the given map!";
    if (foodX >= mapWidth || foodY >= mapHeight || foodX < 0 || foodY < 0)
        throw "Invalid food coordinates for the given map!";

    monster = &map[monsterY][monsterX];
    monster->symbol = MONSTER;
    monster->color = monsterColor;


    food = &map[foodY][foodX];
    food->symbol = FOOD;
    food->color = foodColor;

}

// Loads the map from csv file. If the file is not valud returns false.
// Auto-allocates the position for the monster and the food.
inline bool Puzzle::loadMap(const char * fileName)
{
    std::ifstream in(fileName);
    if (!in)
        return false;

    char ch;
    getMatrixBounds(mapWidth, mapHeight, in);
    vfbCellWidth = RESX / (mapWidth + 1); // For now just a simple bounds
    vfbCellHeight = RESY / (mapHeight + 1);

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

            // TO DO: ma
            if (ch == WALL)
            {
                map[i][j].color = wallColor;
            }
            else if (ch == FREE)
            {
                map[i][j].color = freeColor;
            }
            else if (ch == WATER)
            {
                map[i][j].color = waterColor;
            }
            else if(ch != '\n' && ch != EOF)
                throw string("In the given map there is some invalid symbol '") ;//+ ch + string("' at position (") + to_string(i) + string(",") + to_string(j) + string(")!");
        }
    }

    in.clear();
    in.close();
    return true;
}

// Prints the map on the given ostream.
inline void Puzzle::printMap(std::ostream& out) const
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

// Solves the puzzle and sets parents of the cells.
// After this function, I can go from the FOOD cell by it`s parents to get the generated path.
inline void Puzzle::solve(std::ostream& out)
{
    if (!monster || !food)
        throw "Invalid monster or food cells!";

    // Reset all cell`s search data (visited and parent fields).
    resetValuesOfTheCells();

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

        pushCellsChildren(current, front, out);
    }
}

// Makes the path cells '*'.
// Prints the map and after that prints the formated path.
inline void Puzzle::basicVisualizePath(ostream& out)
{
    Cell * cell = food->parent;
    if (cell)
        while (cell->parent)
        {
            cell->symbol = '*';
            cell = cell->parent;
        }

    printMap(out);
    printFormatedPath(out);
}

// Prints the path from the monster to his food.
inline void Puzzle::printFormatedPath(ostream& out) const
{
    out << "[";
    printFormatedPath(food, out);
    out << "]\n";
}

// Prints the path from the monster to his food.
inline void Puzzle::printFormatedPath(Cell * cell, ostream& out) const
{
    if (!cell)
        return;

    printFormatedPath(cell->parent, out);
    out << "(" << cell->x << "," << cell->y << ")";
    if (cell != food)
        out << ", ";
}
// Pushes the children of the given Cell to the given priority queue.
inline void Puzzle::pushCellsChildren(Puzzle::Cell * current, priority_queue<Puzzle::Cell*, vector<Puzzle::Cell*>, Puzzle::CellComparison> & front, ostream& out)
{
    Cell * child = NULL;

    // Lest check all neighbour cells. (NOTE: the case where @i and @j are zero, the current cell is visited, so nothing will happen.)
    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j)
        {
            child = getCellAt(current->x + i, current->y + j);
            if (child) // If there is child there(valid cell at this position)
            {
                double newPriceWalkedBlocks = current->priceWalkedBlocks;
                // Let`s calculate the cost of the move to this child.
                if (current->symbol == WATER) // cost to get out from water cell in any direction is fixed - @waterCost
                    newPriceWalkedBlocks += waterCost;
                else if (child->x == current->x || child->y == current->y) // If it`s a direct move(left, right, up or down)
                    newPriceWalkedBlocks += directCost;
                else // it`s diagonal move
                    newPriceWalkedBlocks += diagonalCost;

                // Let`s see if we need to add this child to the @front
                if (!(i == 0 && j == 0) && (!child->visited || (child->visited && child->priceWalkedBlocks > newPriceWalkedBlocks)))
                {
                    child->visited = true;
                    calculateCellDistanceToFood(child);
                    child->parent = current;
                    child->priceWalkedBlocks = newPriceWalkedBlocks;

                    // Add the child to the priority queue.
                    //out << "Inserting child at (" << child->x << "," << child->y << ") with price: " << child->pricePotentialToFood + child->priceWalkedBlocks << "\n";
                    /* Make the color of the cell darker and display it*/
                    child->color *= 0.8;
                    fillVFBCell(child);
                    displayVFB(vfb);
                    front.push(child);
                }
            }
        }
}

// Returns a pointer to the cell with the given coords.
inline Puzzle::Cell * Puzzle::getCellAt(int x, int y)
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight || // check if the cell is outside of the map
        map[y][x].symbol == WALL)// check if the cell is wall or the cell is already visited
        //|| map[y][x].visited)
        return NULL;

    return &map[y][x];
}

// Calculates AND SETS the distacne to the Food.
// Goest as much as possible blockes diagonally and other once direct.
inline void Puzzle::calculateCellDistanceToFood(Puzzle::Cell* cell)
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


/*

    SDL stuff (mostly)
*/



// SDL visualization with path finding
inline void Puzzle::solveAndVizualize(ostream& out)
{
    if (!monster || !food)
        throw "Invalid monster or food cells!";
    // Reset all cell`s search data (visited and parent fields).
    resetValuesOfTheCells();

    // Initial colors for the cells
    fillInitialColors();
    displayVFB(vfb);

    priority_queue<Cell*, vector<Cell*>, Puzzle::CellComparison> front;
    monster->visited = true;
    front.push(monster);

    Cell * current = NULL;

    while (!front.empty())
    {
        // Let`s get the 'best' node
        current = front.top();
        front.pop();
        //out << "Looking cell at (" << current->x << "," << current->y << ") with price: " << current->pricePotentialToFood + current->priceWalkedBlocks << "\n";

        /* Make the color of the cell darker and display it*/
        /*  Delay between every cell pop*/
        Sleep(delay);
        current->color *= 0.7;
        fillVFBCell(current);
        displayVFB(vfb);

        // If we found the food, breaks.
        if (current == food)
        {
            out << "\n\nPrice to the food is: " << current->priceWalkedBlocks << "\n"; // Price to the food is zero...
            break;
        }

        pushCellsChildren(current, front, out);
    }
}

// Visualize the path from the FOOD cell by it`s parents to the MONSTER.(Makes the color of the cells brawn)
inline void Puzzle::visualizeThePath()
{
    visualizeThePath(food);
}

// Visualize the closest path from the Monster to the cell
inline void Puzzle::visualizeThePath(Cell * cell)
{
    if (!cell)
        return;

    visualizeThePath(cell->parent);

    // Make the color of the cell like the monster one*
    cell->color = monsterColor;
    fillVFBCell(cell);
    /* Delay between every cell visualize */
    Sleep(delay << 1); // Multiply the basic delay by 4
    displayVFB(vfb);
}

// Goes through every cell and makes needed color to the virtual frame buffer
inline void Puzzle::fillInitialColors()
{
    for (int i = 0; i < mapHeight; ++i)
        for (int j = 0; j < mapWidth; ++j)
        {
            fillVFBCell(&map[i][j]);
        }
}

// Fills the virtual frame buffer cell with the given Cells color
inline void Puzzle::fillVFBCell(Cell * cell)
{
    if (!cell)
        return;

    int vfbYPositionStart = (cell->y) * vfbCellHeight;
    int vfbYPositionEnd = vfbYPositionStart + vfbCellHeight;
    int vfbXPositionStart = (cell->x) * vfbCellWidth;
    int vfbXPositionEnd = vfbXPositionStart + vfbCellWidth;

    for (int y = vfbYPositionStart; y < vfbYPositionEnd; ++y)
        for (int x = vfbXPositionStart; x < vfbXPositionEnd; ++x)
        {
            vfb[y][x] = cell->color;
        }
}






/*
    End of SDL stuff (mostly)
*/

// Sets the default values of the puzzle members.
inline void Puzzle::setDefaultValues()
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
    wallColor = Color(0x000000);
    freeColor = Color(0x00FF00);
    waterColor = Color(0x00D0FF);
    monsterColor = Color(0xCF5B1E);
    foodColor = Color(0x9D5BA6);
    vfbCellWidth = vfbCellHeight = 0;
    delay = 0;
}

// Reset all cell`s visited and parents values.
inline void Puzzle::resetValuesOfTheCells()
{
    for (int i = 0; i < mapHeight; ++i)
        for (int j = 0; j < mapWidth; ++j)
        {
            map[i][j].visited = false;
            map[i][j].parent = NULL;
        }
}

#endif
