#ifndef COINGAME__H_
#define COINGAME__H_

/*	Name: Anton Vasilev Dudov
 *	FN:   71488
 *
 *	Homework two - coins game with alpha-beta cut
 *
 *	I used some different child generations and I found out that for the starting with cylce of N coins, the generation of childs which is 
 *	taking three coins from every position, after that taking two coins from every positions and at the end one coin is the best, because it 
 *	makes the tree of all states shorter.
 *
 *
 *	Github repository: https://github.com/Anton94/Systems-based-on-knowledge/tree/master/Homework%20two%20MinMaxAphaBetaCut
 *	
 *
 *	If you have any questions, you can ask me via mail: antonvdudov@gmail.com
 */

#include <iostream>
#include <vector>
#include <limits>

using std::vector;

class CoinGame
{
	// Why I use nodes? 
	// ...So I can trace the result and find the exact player`s moves so it`s not only the score, but how they get to that score.
	struct Node // State of the field
	{
		static int size; // Number of coins.
		vector<char> coins; // Or dynamic array mine...
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

	CoinGame::Node * root;
	int minusInf;
	int inf;
	void (CoinGame::*childGeneration)(CoinGame::Node * node, vector<CoinGame::Node*>& child) const; // There are different ways to generate childs. I choose one of them and use it for my alpha-beta cuts.
public:
	CoinGame(size_t n);
	~CoinGame();

	// Generate the result and prints it to the given ostream @out.
	void generate(std::ostream& out, bool printResult = true);

	// Prints step by step who player which coins has taken and such.
	void printGameInfo(std::ostream& out) const;
private:
	// Print short statistic. 'playCoinGame' number of coins -> {position where the player start taking coins, number of coins, game score}
	void printShortGameInfo(CoinGame::Node * n, bool maximizingTurn, int score, std::ostream& out) const;

	// Prints step by step who player which coins has taken and such.
	void printGameInfo(CoinGame::Node * n, bool maximizingTurn, std::ostream& out) const;

	// Find the index where the player start taking the coins. (NOTE: if the place is 0 or 1, he could start from N - 2 or N - 1)
	// I can add it to the nodes, but the result is tiny bit of the whole program so it will be better to make it this way, so I don`t calculate it every time for the huge ammount of 'cutted' nodes.
	int positionOfTaking(CoinGame::Node * parent, CoinGame::Node * child) const;

	// Difference between number of coins of the two given states.
	int takenCoinsDiff(CoinGame::Node * parent, CoinGame::Node * child) const;

	// The current state(@n), number of moves (@moves), is MAX on turn (@maximizingTurn), maximum value (@alpha), minimum value (@beta)
	int alphaBeta(CoinGame::Node * n, int moves, bool maximizingTurn, int alpha, int beta);

	// Generate the child. 
	// Try to take coin from every position and try to take one if it has, try 2 and try 3 coins (coins are taken from 'i-th' position and to the right of it)
	void generateChildSmallToBig(CoinGame::Node * node, vector<CoinGame::Node*>& child) const;
	
	// Generate the child. 
	// Try to take coin from every position and try to take three if it has 3 side-by-side, try 2 or 1 (coins are taken from 'i-th' position and to the right of it)
	void generateChildBiGToSmall(CoinGame::Node * node, vector<CoinGame::Node*>& child) const;
	
	// Generate the child. 
	// Try to take coin from every position 3 coins, after that try 2 coins, and at the end one coin
	void generateChildThreeTowOne(CoinGame::Node * node, vector<CoinGame::Node*>& child) const;

	// Generate the child. 
	// Try to take coin from every position 1 coin, after that try 2 coins, and at the end three coins
	void generateChildOneTwoThree(CoinGame::Node * node, vector<CoinGame::Node*>& child) const;
private:
	// Ban copy constructor and operator=. I don`t need them for now.
	CoinGame(const CoinGame& other);
	CoinGame& operator=(const CoinGame& other);
};



#endif