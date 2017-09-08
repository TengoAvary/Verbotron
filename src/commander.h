#ifndef commander_h
#define commander_h

#include "chess.h"

class Commander {
	
private:
	
	// the command most recently recevied from the uci. does not include \n at the end.
	std::string command;
	
	// whether or not to run in uci mode.
	bool uci_mode;
	
public:
	
	// constructor.
	Commander();
	
	// get instruction from command line.
	void get_command();
	
	// send command to command line.
	void print_command();
	
	// process most recent command
	void process();
	
};

#endif /* square_h */