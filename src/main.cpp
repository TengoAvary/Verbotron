#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cassert>
#include <algorithm>

#include "chess.h"

int main(int argc, const char * argv[]) {
	
	Board board;
	
	// board.get_FEN("7k/8/8/2n5/8/3K4/8/8 w - - 0 1");
	
	board.print();
	
	//std::vector<Move> moves = board.get_moves();
	
	//for (int i = 0; i<moves.size(); i++) {
	//	std::cout << board.move_to_str(moves[i]) << "\n";
	//}
	
	//std::vector<Square> attsqu = board.attacking_squares({2,5}, true, false);
	//for (int i = 0; i<attsqu.size(); i++) {
	//	std::cout << attsqu[i].rank << attsqu[i].file << "\n";
	//}
	
	std::vector<Square> test;
	test.push_back({1,1});
	Square item = {2,2};
	test.push_back(item);
	
	
	Square thing = *std::find(test.begin(), test.end(), item);
	std::cout << thing.rank << thing.file << '\n';
	
}
