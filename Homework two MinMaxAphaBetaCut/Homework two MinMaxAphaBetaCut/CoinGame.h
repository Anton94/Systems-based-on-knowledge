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
		int coinsLeft;

		Node();
		Node(const Node& other);
		Node& operator=(const Node& other);
		~Node();
		bool hasCoins() const;
	private:
		void copyFrom(const Node& other);
		void freeMemoryForCoinsAndChild(); // Cascade destructor for all childs.
	};

	Node * root;

public:
	CoinGame(size_t n);
	~CoinGame();

	// Generate the result and prints it to the given ostream @out.
	void generate(std::iostream& out)
	{
		
	}

private:
	// The current state(@n), number of moves (@moves), is MAX on turn (@maximizingTurn), maximum value (@alpha), minimum value (@beta)
	int alphaBeta(Node * n, int moves, bool maximizingTurn, int alpha, int beta)
	{
		return 42;
	}

	// Generate the child. 
	// Try to take coin from every position and try to take one if it has, try 2 and so .. try 3 coins (coins are taken from 'i-th' position and to the right of it)
	void generateChild(Node * node, vector<Node*>& child) const
	{
		Node * tempChild = NULL;
		int iPlusOne = 0, iPlusTwo = 0;

		for (int i = 0; i < CoinGame::Node::size; ++i)
		{
			if (node->coins[i])
			{
				// Take one coin at i-th position
				tempChild = new Node(*node);
				tempChild->coins[i] = 0;
				--(tempChild->coinsLeft);
				child.push_back(tempChild);

				iPlusOne = (i + 1) % CoinGame::Node::size;
				if (node->coins[iPlusOne])
				{
					// Take two coins starting from i-th position (to right)
					tempChild = new Node(*tempChild); // Copies the child without coin at position 'i'
					tempChild->coins[iPlusOne] = 0;
					--(tempChild->coinsLeft);
					child.push_back(tempChild);

					iPlusTwo = (i + 2) % CoinGame::Node::size;
					if (node->coins[iPlusTwo])
					{
						tempChild = new Node(*tempChild); // Copies the child without coin at position 'i' and 'iPlusOne'
						tempChild->coins[iPlusTwo] = 0;
						--(tempChild->coinsLeft);
						child.push_back(tempChild);
					}
				}
			}
		}
	}
};



#endif