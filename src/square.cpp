#include "chess.h"

bool operator==(const Square& one, const Square& two) {
		return ((one.rank == two.rank) && (one.file == two.file));
	}