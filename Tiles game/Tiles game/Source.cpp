#include <iostream>
#include "TilesPuzzle.h"


int Hash::BASE1 = 3607;
int Hash::BASE2 = 3613;
int Hash::BASE3 = 3617;

int Hash::MOD1 = 1000003;
int Hash::MOD2 = 1000000021;
int Hash::MOD3 = 1000000181;

size_t TilesPuzzle::MAX_HASH = 1000003;

/*
	Anton Dudov #71488

	Tiles puzzle, for now good up to 60x60 board, check BASEs of the hash to resize it.
*/

int main()
{
	vector<unsigned short> v;
	for (size_t i = 0; i < 81; ++i)
		v.push_back(i);

	std::swap(v[0], v[1]);
	TilesPuzzle puzzle(v, std::cout);
	return 0;
}