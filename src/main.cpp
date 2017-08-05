#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cassert>

#include "chess.h"

int main(int argc, const char * argv[]) {
	
	Board board;
	
	board.get_FEN("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
	
	board.print();
	
	board.get_moves();
	
}
