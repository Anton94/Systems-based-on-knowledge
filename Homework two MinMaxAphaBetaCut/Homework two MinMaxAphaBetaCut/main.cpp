#include <iostream>
#include "CoinGame.h"

int main()
{
	// Checks for memory leaks. Works only in VS (for example) the memory check.
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
	{
		for (int i = 1; i <= 10; ++i)
		{
			CoinGame game(i);
			game.generate(std::cout);
		}
		for (int i = 13; i <= 13; ++i)
		{
			CoinGame game(i);
			game.generate(std::cout, false);
			game.printGameInfo(std::cout);
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