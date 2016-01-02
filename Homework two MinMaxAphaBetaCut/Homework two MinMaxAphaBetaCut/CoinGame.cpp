#include "CoinGame.h"

/*
 *	Node: constructors and destructor.
*/
CoinGame::Node::Node()
{
	coins = new char[size];
	for (int i = 0; i < size; ++i)
		coins[i] = 1;
	bestChild = NULL;
}

CoinGame::Node::Node(const Node& other)
{
	copyFrom(other);
}

CoinGame::Node& CoinGame::Node::operator=(const Node& other)
{
	std::cout << "I don`t want to use this, but let`s we have it!" << std::endl;
	if (this != &other)
	{
		freeMemoryForCoinsAndChild();
		copyFrom(other);
	}

	return *this;
}

CoinGame::Node::~Node()
{
	freeMemoryForCoinsAndChild();
}

void CoinGame::Node::copyFrom(const Node& other)
{
	coins = new char[size];
	for (int i = 0; i < size; ++i)
		coins[i] = other.coins[i];
	bestChild = NULL;
}

void CoinGame::Node::freeMemoryForCoinsAndChild() // Cascade destructor for all childs.
{
	delete[] coins;
	delete bestChild;
}

/*
 *
*/

// Initialization of the default number of coins (3)
int CoinGame::Node::size = (int)3;

/*
*	CoinGame: constructor and destructor.
*/
CoinGame::CoinGame(size_t n)
{
	root = new Node();
	CoinGame::Node::size = (int)n;
}

CoinGame::~CoinGame()
{
	delete root; // Cascade node destructor
}

/*
*
*/