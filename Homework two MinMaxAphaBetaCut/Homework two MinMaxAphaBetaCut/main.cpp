#include <iostream>
#include <fstream>
#include "CoinGame.h"

int main()
{
	// Checks for memory leaks. Works only in VS (for example) the memory check.
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
	{
		std::ofstream out("output.txt");
		for (int i = 1; i <= 12; ++i)
		{
			CoinGame game(i);
			game.generate(out);
		}
		for (int i = 1; i <= 12; ++i)
		{
			CoinGame game(i);
			game.generate(out, false);
			game.printGameInfo(out);
		}
		out.close();
	}

	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		std::cout << "Memory leak detected!" << std::endl;
		_CrtMemDumpStatistics(&s3);
	}
	return 0;
}