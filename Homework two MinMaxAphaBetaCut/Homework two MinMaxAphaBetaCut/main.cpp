#include <iostream>
#include "CoinGame.h"

int main()
{
	
	// Checks for memory leaks. Works only in VS (for example) the memory check.
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
	{
		for (int i = 1; i <= 15; ++i)
		{
			CoinGame game(i);
			std::cout << "Coins " << i << " ";
			game.generate(std::cout);
		}
	}

	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		std::cout << "Memory leak detected!" << std::endl;
		_CrtMemDumpStatistics(&s3);
	}
	return 0;
}