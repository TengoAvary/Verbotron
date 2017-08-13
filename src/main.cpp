#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cassert>
#include <algorithm>

#include "chess.h"

int main(int argc, const char * argv[]) {
	
	Board board;
	
	board.get_FEN("r3k3/4r3/8/8/8/8/8/R3K2R w KQkq - 0 1");
	
	board.print();
	
	std::vector<Move> moves = board.get_moves();
	
	for (int i = 0; i<moves.size(); i++) {
		std::cout << board.move_to_str(moves[i]) << "\n";
	}
	
	//std::vector<Square> attsqu = board.attacking_squares({2,5}, true, false);
	//for (int i = 0; i<attsqu.size(); i++) {
	//	std::cout << attsqu[i].rank << attsqu[i].file << "\n";
	//}
	
}
