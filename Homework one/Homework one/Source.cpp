#include <iostream>
#include "Puzzle.h"

class MyNumbers
{
public:
	int a;
	MyNumbers(int a = 0)
	{
		this->a = a;
	}
	bool operator>(const MyNumbers& rhs) const
	{
		return a > rhs.a;
	}
};

class CellComparison
{
public:
	bool operator() (const MyNumbers* lhs, const MyNumbers* rhs) const
	{
		return *lhs > *rhs;
	}
};

int main()
{
	try
	{
		Puzzle puzzle;
		if (!puzzle.loadMap("map1.csv"))
			throw "Something is wrong with the map file!";
		puzzle.printMap(std::cout);
	}
	catch (const char * msg)
	{
		std::cout << "Error: " << msg << std::endl;
	}
	catch (const string msg)
	{
		std::cout << "Error: " << msg << std::endl;
	}


	priority_queue<MyNumbers*, vector<MyNumbers*>, CellComparison> pq;
	MyNumbers arr[15];

	for (int i = 0; i < 15; ++i)
		arr[i] = rand() % 1000;


	for (int i = 0; i < 15; ++i)
		pq.push(&arr[i]);

	for (int i = 0; i < 15; ++i)
	{
		cout << pq.top()->a << " ";
		pq.pop();
	}
	cout << "\n";

	return 0;
}