#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cassert>

#include "chess.h"

int main(int argc, const char * argv[]) {
	
	Board board;
	
	board.get_FEN("8/3p4/8/q2K4/8/4n3/8/8 w - - 0 1");
	
	board.print();
	
	std::vector<Square> attacking = board.attacking_squares({4,3}, false, false);
	for (int i = 0; i<attacking.size(); i++) {
		std::cout << attacking[i].rank << attacking[i].file << "\n";
	}
	
}
