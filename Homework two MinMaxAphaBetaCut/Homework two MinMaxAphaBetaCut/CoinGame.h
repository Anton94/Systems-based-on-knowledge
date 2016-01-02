#ifndef COINGAME__H_
#define COINGAME__H_

#include <iostream>
#include <vector>
#include <limits>

using std::vector;

/// TO DO: 
//			- Proper output
//			- Check -3 magic number
//			- Add time
//			- Add fancy output

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
	int minusInf;
	int inf;
public:
	CoinGame(size_t n);
	~CoinGame();

	// Generate the result and prints it to the given ostream @out.
	void generate(std::ostream& out)
	{
		if (CoinGame::Node::size <= 3)
		{
			out << "Score = " << (CoinGame::Node::size - 1) << "\n";
			return;
		}

		int score = alphaBeta(root, 0, true, minusInf, inf);
		out << "Score = " << score << "\n";

		
		//printGameInfo(root, true, out);
	}

private:
	void printGameInfo(Node * n, bool maximizingTurn, std::ostream& out) const
	{
		if (!n)
			return;

		out << "\tCoins : [";
		for (int i = 0; i < CoinGame::Node::size; ++i)
			out << " " << (int)n->coins[i];
		out << "]";

		if (maximizingTurn)
			out << "\nMAX turn:\n";
		else
			out << "\nMIN turn:\n";

		printGameInfo(n->bestChild, !maximizingTurn, out);
	}


	// The current state(@n), number of moves (@moves), is MAX on turn (@maximizingTurn), maximum value (@alpha), minimum value (@beta)
	int alphaBeta(Node * n, int moves, bool maximizingTurn, int alpha, int beta)
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
				{
					delete child[i];
				}

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
				{
					delete child[i];
				}

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
	void generateChild(Node * node, vector<CoinGame::Node*>& child) const
	{
		Node * tempChild = NULL;
		int iPlusOne = 0, iPlusTwo = 0;

		/*std::cout << "BLABLA : [";
		for (int i = 0; i < CoinGame::Node::size; ++i)
		{
			if ((int)node->coins[i] != 0 && (int)node->coins[i] != 1)
				std::cout << " FOCK " << std::endl;
			std::cout << " " << (int)node->coins[i];
		}
		std::cout << "]";*/
		for (int i = 0; i < CoinGame::Node::size; ++i)
		{
			if (node->coins[i] != 0)
			{
				// Take one coin at i-th position
				tempChild = new Node(*node);
				tempChild->coins[i] = (int)0;
				if (tempChild->coins[i] != 0 && tempChild->coins[i] != 1)
					std::cout << "HEREE" << std::endl;

				tempChild->coinsLeft = tempChild->coinsLeft - 1;
				child.push_back(tempChild);

				iPlusOne = (i + 1) % CoinGame::Node::size;
				//std::cout << "i = " << i << " " << iPlusOne << std::endl;
				if (node->coins[iPlusOne])
				{
					// Take two coins starting from i-th position (to right)
					tempChild = new Node(*node); // Copies the child without coin at position 'i'
					tempChild->coins[iPlusOne] = (int)0;
					tempChild->coins[i] = (int)0;
					if (tempChild->coins[i] != 0 && tempChild->coins[i] != 1)
						std::cout << "HEREE" << std::endl;
					tempChild->coinsLeft = tempChild->coinsLeft - 1;
					child.push_back(tempChild);

					iPlusTwo = (i + 2) % CoinGame::Node::size;

					//std::cout << "i = " << i << " " << iPlusOne << " " << iPlusTwo << std::endl;
					if (node->coins[iPlusTwo])
					{
						tempChild = new Node(*node); // Copies the child without coin at position 'i' and 'iPlusOne'
						tempChild->coins[iPlusTwo] = (int)0;
						tempChild->coins[iPlusOne] = (int)0;
						tempChild->coins[i] = (int)0;
						if (tempChild->coins[i] != 0 && tempChild->coins[i] != 1)
							std::cout << "HEREE" << std::endl;
						tempChild->coinsLeft = tempChild->coinsLeft - 1;
						child.push_back(tempChild);
					}
				}
			}
		}
	}
private:
	// Ban copy constructor and operator=. I don`t need them for now.
	CoinGame(const CoinGame& other);
	CoinGame& operator=(const CoinGame& other);
};



#endif