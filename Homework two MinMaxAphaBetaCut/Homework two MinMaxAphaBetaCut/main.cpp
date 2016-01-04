#include <iostream>
#include <fstream>
#include "CoinGame.h"

int main()
{
	// Checks for memory leaks. Works only in VS (for example) the memory check.
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
	{
		int n = 0;
		std::cin >> n;
		
		if (n <= 0)
		{
			std::cout << "N is less than 1, wrong input!\n";
			return 0;
		}

		CoinGame game(n);
		game.generate(std::cout);

		/*
		std::ofstream out("output.txt");
		for (int i = 1; i <= 15; ++i)
		{
			CoinGame game(i);
			game.generate(out);
		}
		for (int i = 1; i <= 15; ++i)
		{
			CoinGame game(i);
			game.generate(out, false);
			game.printGameInfo(out);
		}
		out.close();*/

		//// Proper homework output design
		//for (int i = 1; i <= 15; ++i)
		//{
		//	CoinGame game(i);
		//	game.generate(std::cout);
		//}
	}

	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		std::cout << "Memory leak detected!" << std::endl;
		_CrtMemDumpStatistics(&s3);
	}
	return 0;
}