#pragma once

#include <vector>
#include <utility>
#include <queue>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <chrono>

using namespace std::chrono;
using std::vector;
using std::queue;
using std::ostream;

typedef unsigned short tiles;

struct Hash
{
	static int BASE1;
	static int BASE2;
	static int BASE3;

	static int MOD1;
	static int MOD2;
	static int MOD3;
	int h1, h2, h3;

	Hash()
	{
		h1 = h2 = h3 = 1;
	}

	Hash(const vector<tiles>& board)
	{
		h1 = h2 = h3 = 1;

		boardHash(board);
	}

	bool operator==(const Hash& r) const
	{
		return h1 == r.h1 && h2 == r.h2 && h3 == r.h3;
	}

	void boardHash(const vector<tiles>& board)
	{
		int boardSize = (int)board.size();

		for (int i = 0; i < boardSize; i++)
		{
			h1 = ((long long)h1 * BASE1 + board[i]) % MOD1;
			h2 = ((long long)h2 * BASE2 + board[i]) % MOD2;
			h3 = ((long long)h3 * BASE3 + board[i]) % MOD3;
		}
	}
};



class TilesPuzzle
{
public:
	static size_t MAX_HASH;
private:
	struct Node
	{
		vector<tiles> board;
		vector<Node*> childs;
		Node * parent;
		Hash hash;
		size_t position;
	public:
		Node(const vector<tiles>& board, Node * parent, size_t position)
		{
			this->board = board;
			this->parent = parent;
			this->position = position;
		}

		void generateHash()
		{
			this->hash.boardHash(board);
		}

		vector<tiles> getBoard() const // NOTE: Returns a copy.
		{
			return board;
		}

		~Node()
		{
			size_t size = childs.size();
			for (size_t i = 0; i < size; ++i)
				delete childs[i];
		}
	};
private:
	Node * root;
	size_t numberOfRows; // rows on power of 2 is the number of tails + empty spot('0').
	size_t numberOfTiles;
	Hash goalHash;
public:
	TilesPuzzle(const vector<tiles>& startBoard, ostream& out)
	{
		auto begin = high_resolution_clock::now();

		root = new Node(startBoard, NULL, allocatePosition(startBoard));
		root->generateHash();
		numberOfTiles = startBoard.size();
		numberOfRows = sqrt(numberOfTiles);
		generateGoalHash(startBoard.size());
		Node * n = solve(out); // Auto solve

		auto end = high_resolution_clock::now();
		auto ticks = duration_cast<microseconds>(end - begin);
		
		size_t steps = 0;
		printPath(n, out, steps);

		std::cout << "It took me ";
		if (n != NULL) // The puzzle is solved
			out << steps << " step(s) and ";
			
		out << ticks.count() << " microseconds. \n";
	}

	Node * solve(std::ostream & out)
	{
		vector<char> hashMap(MAX_HASH, 0); // I will keep my nodes by first hash. TO DO make containers and such...
		queue <Node*> q;

		hashMap[root->hash.h1] = 1;
		q.push(root);

		while (!q.empty())
		{
			Node * current = q.front();
			q.pop();

			if (isGoal(current->hash))
			{
				return current;
			}

			generateChildrens(current);

			for (size_t i = 0; i < current->childs.size(); ++i)
			{
				if (hashMap[current->childs[i]->hash.h1] == 0)
				{
					q.push(current->childs[i]);
					hashMap[current->childs[i]->hash.h1] = 1;
				}
			}
		}

		out << "I can`t solve it :(\nI am prety sure that it is not solvable!\n";

		return NULL;
	}

	~TilesPuzzle()
	{
		delete root;
	}
private:
	size_t allocatePosition(const vector<tiles>& board) const
	{
		size_t size = board.size();

		for (size_t i = 0; i < size; ++i)
		{
			if (board[i] == 0)
				return i;		
		}

		return -1; // 'infinity'
	}

	void generateChildrens(Node * n)
	{
		// Try to generate childs when swaping the empty space(zero) with UP, DOWN, LEFT and RIGHT tiles.
		
		// UP
		if (n->position >= numberOfRows)
			generateChild(n, n->position - numberOfRows); 
		
		// DOWN
		if (n->position + numberOfRows < numberOfTiles)
			generateChild(n, n->position + numberOfRows);
		
		// LEFT
		if (n->position % numberOfRows != 0)
			generateChild(n, n->position - 1);		

		// RIGHT
		if (n->position % numberOfRows != numberOfRows - 1)
			generateChild(n, n->position + 1);		
	}

	void generateChild(Node * n, size_t position)
	{
		if (position >= 0 && position  < numberOfTiles)
		{
			Node * child = new Node(n->board, n, position);
			std::swap(child->board[child->position], child->board[n->position]);
			child->generateHash();

			n->childs.push_back(child);
		}
	}

	void generateGoalHash(tiles size)
	{
		vector<tiles> goalBoard(size);
		
		for (tiles i = 0; i < size; ++i)
			goalBoard[i] = i;

		goalHash.boardHash(goalBoard);
	}

	bool isGoal(const Hash& hash) const
	{
		return hash == goalHash;
	}

	void printPath(Node * n, std::ostream & out, size_t& steps) const
	{
		if (!n)
			return;

		if (n->parent)
		{
			printPath(n->parent, out, steps);
			++steps;
		}

		size_t size = n->board.size();
		for (size_t i = 0; i < size; ++i)
		{
			out << std::setw(3) << n->board[i];
			if ((i + 1) % numberOfRows == 0)
				out << "\n";
		}

		out << "\n";
	}
};