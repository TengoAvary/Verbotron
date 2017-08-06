#ifndef square_h
#define square_h

#include "chess.h"

struct Square {
	
	int rank;
	int file;
	int direction; // used in get_moves function
	
};

bool operator==(const Square& one, const Square& two);

#endif /* square_h */