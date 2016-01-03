#include "CoinGame.h"

/*
 *	Node: constructors and destructor.
*/
CoinGame::Node::Node()
{
	coins.resize(CoinGame::Node::size);
	for (int i = 0; i < CoinGame::Node::size; ++i)
		coins[i] = 1;

	bestChild = NULL;
	coinsLeft = CoinGame::Node::size;
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
	coins = other.coins;
	bestChild = NULL;
	coinsLeft = other.coinsLeft;
}

void CoinGame::Node::freeMemoryForCoinsAndChild() // Cascade destructor for all childs.
{
	delete bestChild;
}

/*
*
*/


// Initialization of the default number of coins (3)
int CoinGame::Node::size = (int)1000;

/*
*	CoinGame: constructor and destructor.
*/
CoinGame::CoinGame(size_t n)
{
	CoinGame::Node::size = (int)n;
	root = new Node();
	minusInf = std::numeric_limits<int>::min();
	inf = std::numeric_limits<int>::max();
}

CoinGame::~CoinGame()
{
	delete root; // Cascade node destructor
}
// Generate the result and prints it to the given ostream @out.
void CoinGame::generate(std::ostream& out, bool printResult)
{
	if (CoinGame::Node::size <= 3)
	{
		if (printResult)
		{
			out << "playCoinGame " << CoinGame::Node::size << " -> {"
				<< 0 << " ,"
				<< CoinGame::Node::size << " ,"
				<< (CoinGame::Node::size - 1) << "}\n";
		}
		return;
	}

	int score = alphaBeta(root, 0, true, minusInf, inf);

	if (printResult)
		printShortGameInfo(root, true, score, out);
}

// Prints step by step who player which coins has taken and such.
void CoinGame::printGameInfo(std::ostream& out) const
{
	printGameInfo(root, true, out);
}

// Print short statistic. 'playCoinGame' number of coins -> {position where the player start taking coins, number of coins, game score}
void CoinGame::printShortGameInfo(CoinGame::Node * n, bool maximizingTurn, int score, std::ostream& out) const
{
	out << "playCoinGame " << CoinGame::Node::size << " -> {"
		<< positionOfTaking(n, n->bestChild) << " ,"
		<< takenCoinsDiff(n, n->bestChild) << " ,"
		<< score << "}\n";
}

// Prints step by step who player which coins has taken and such.
void CoinGame::printGameInfo(Node * n, bool maximizingTurn, std::ostream& out) const
{
	if (!n)
		return;

	out << "\tCoins : [";
	for (int i = 0; i < CoinGame::Node::size; ++i)
		out << " " << (int)n->coins[i];
	out << "]";

	out << "\n";
	if (!n->bestChild)
		return; // So it wont print MAX turn again(or MIN turn).
	if (maximizingTurn)
		out << "MAX ";
	else
		out << "MIN ";

	out << "turn taking " << takenCoinsDiff(n, n->bestChild) << " coins from position " << positionOfTaking(n, n->bestChild) << ":\n";

	printGameInfo(n->bestChild, !maximizingTurn, out);
}

// Find the index where the player start taking the coins. (NOTE: if the place is 0 or 1, he could start from N - 2 or N - 1)
// I can add it to the nodes, but the result is tiny bit of the whole program so it will be better to make it this way, so I don`t calculate it every time for the huge ammount of 'cutted' nodes.
int CoinGame::positionOfTaking(CoinGame::Node * parent, CoinGame::Node * child) const
{
	int coinsNumber = CoinGame::Node::size;
	for (int i = 0; i < CoinGame::Node::size; ++i)
	{
		// If there is diff between coins values (0 or 1)
		if (parent->coins[i] != child->coins[i])
		{
			// If there is difference (at the end of the coins(last position or it`s prev (last prev)
			if (parent->coins[(i + coinsNumber - 1) % coinsNumber] != child->coins[(i + coinsNumber - 1) % coinsNumber])
			{
				if (parent->coins[(i + coinsNumber - 2) % coinsNumber] != child->coins[(i + coinsNumber - 2) % coinsNumber])
				{
					return (i + coinsNumber - 2) % coinsNumber;
				}

				return (i + coinsNumber - 1) % coinsNumber;
			}

			return i;
		}
	}

	return -1;//Error or something.
}

// Difference between number of coins of the two given states.
int CoinGame::takenCoinsDiff(CoinGame::Node * parent, CoinGame::Node * child) const
{
	return parent->coinsLeft - child->coinsLeft;
}

// The current state(@n), number of moves (@moves), is MAX on turn (@maximizingTurn), maximum value (@alpha), minimum value (@beta)
int CoinGame::alphaBeta(Node * n, int moves, bool maximizingTurn, int alpha, int beta)
{
	// If the game is over than the previous player took last coins, so he wins the points.
	if (n->coinsLeft <= 0)
	{
		int score = CoinGame::Node::size - moves;
		return maximizingTurn ? -score : score; // If it`s MAX turn, so MIN took last coin(s) and the result should be negative.
	}

	// Let`s generate all cases where the coins can be taken and make them as states(Nodes)
	vector<CoinGame::Node *> child;
	generateChild(n, child);

	int childNumber = child.size();
	if (child.size() == 0)
	{
		int score = CoinGame::Node::size - moves;
		return maximizingTurn ? -score : score; // If it`s MAX turn, so MIN took last coin(s) and the result should be negative.
	}
	int score, tempScore;
	Node* bestChild = NULL;

	if (maximizingTurn)
	{
		score = minusInf;
		for (int i = 0; i < childNumber; ++i)
		{
			tempScore = alphaBeta(child[i], moves + 1, false, alpha, beta);
			if (score < tempScore)
			{
				score = tempScore;
				delete bestChild;
				bestChild = child[i];
			}
			else
				delete child[i];

			alpha = (alpha < score) ? score : alpha;

			if (beta <= alpha)
			{
				for (int j = i + 1; j < childNumber; ++j)
					delete child[j];
				break;
			}
		}
	}
	else
	{
		score = inf;
		for (int i = 0; i < childNumber; ++i)
		{
			tempScore = alphaBeta(child[i], moves + 1, true, alpha, beta);
			if (score > tempScore)
			{
				score = tempScore;
				delete bestChild;
				bestChild = child[i];
			}
			else
				delete child[i];

			beta = (beta > score) ? score : beta;

			if (beta <= alpha)
			{
				for (int j = i + 1; j < childNumber; ++j)
					delete child[j];
				break;
			}
		}
	}

	n->bestChild = bestChild;

	return score;
}

// Generate the child. 
// Try to take coin from every position and try to take one if it has, try 2 and so .. try 3 coins (coins are taken from 'i-th' position and to the right of it)
void CoinGame::generateChild(Node * node, vector<CoinGame::Node*>& child) const
{
	Node * tempChild = NULL;
	int iPlusOne = 0, iPlusTwo = 0;

	for (int i = 0; i < CoinGame::Node::size; ++i)
	{
		if (node->coins[i] != 0)
		{
			// Take one coin at i-th position
			tempChild = new Node(*node);
			tempChild->coins[i] = (int)0;
			tempChild->coinsLeft = tempChild->coinsLeft - 1;
			child.push_back(tempChild);

			iPlusOne = (i + 1) % CoinGame::Node::size;
			if (node->coins[iPlusOne])
			{
				// Take two coins starting from i-th position (to right)
				tempChild = new Node(*tempChild); // Copies the child without coin at position 'i'
				tempChild->coins[iPlusOne] = (int)0;
				tempChild->coinsLeft = tempChild->coinsLeft - 1;
				child.push_back(tempChild);

				iPlusTwo = (i + 2) % CoinGame::Node::size;
				if (node->coins[iPlusTwo])
				{
					tempChild = new Node(*tempChild); // Copies the child without coin at position 'i' and 'iPlusOne'
					tempChild->coins[iPlusTwo] = (int)0;
					tempChild->coinsLeft = tempChild->coinsLeft - 1;
					child.push_back(tempChild);
				}
			}
		}
	}
}
/*
*
*/