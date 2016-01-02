#ifndef COINGAME__H_
#define COINGAME__H_

#include <iostream>
#include <vector>

using std::vector;

class CoinGame
{
	// Why I use nodes? 
	// ...So I can trace the result and find the exact player`s moves so it`s not only the score, but how they get to that score.
	struct Node // State of the field
	{
		static int size; // Number of coins.
		char * coins;
		Node * bestChild;

		Node();
		Node(const Node& other);
		Node& operator=(const Node& other);
		~Node();
	private:
		void copyFrom(const Node& other);
		void freeMemoryForCoinsAndChild(); // Cascade destructor for all childs.
	};

	Node * root;

public:
	CoinGame(size_t n);
	~CoinGame();



private:
	int alphaBeta()
	{
		return 42;
	}
};



#endif