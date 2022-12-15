#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <string>
#include "board.hpp"

using std::stoi;
using std::string;

#define COMMAND_NUM 7

class Game  
{
	const char* commands_name[COMMAND_NUM] = {
		"name",
		"version",
		"time_setting",
		"board_setting",
		"ini",
		"get",
		"exit"
	};
public:
	Game(){
		state = new Board();
	};
	~Game(){
		delete state;
	};

	// commands
	void Name(const char* data[], char* response);
	void Version(const char* data[], char* response);
	void Time_setting(const char* data[], char* response);
	void Board_setting(const char* data[], char* response);
	void Ini(const char* data[], char* response);
	void Get(const char* data[], char* response);
	void Exit(const char* data[], char* response);

private:
	Board* state;
};