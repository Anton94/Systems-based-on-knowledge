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

	TODOs hash buckets, file output, file input
*/

int main()
{
	vector<unsigned short> v;
	for (size_t i = 1; i < 9; ++i)
		v.push_back(i);
	v.push_back(0);

	//std::swap(v[0], v[8]);
	TilesPuzzle puzzle(v, std::cout);
	return 0;
}