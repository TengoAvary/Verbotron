#include <iostream>
#include <sys/types.h>
#include <vector>
#include <cassert>

#include "chess.h"

int main(int argc, const char * argv[]) {
	
	Board *board = new Board();
	board -> print();
	
	board -> get_moves();
	
	std::cout << Board::move_vectors[WHITE_KNIGHT][1].rank << Board::move_vectors[WHITE_KNIGHT][1].file;
	
}
