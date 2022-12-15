#include "float.h"
#include "game.hpp"
#include "negascout.hpp"

void Game::Name(const char* data[], char* response)
{
	strcpy(response, "NegaScout");
}

void Game::Version(const char* data[], char* response)
{
	strcpy(response, "1.0.0");
}

void Game::Time_setting(const char* data[], char* response)
{
	state->red_time = stoi(data[1]);
	state->blue_time = stoi(data[1]);
	strcpy(response, "1");
}

void Game::Board_setting(const char* data[], char* response)
{
	state->board_size = stoi(data[1]);
	state->red_piece_num = stoi(data[2]);
	state->blue_piece_num = stoi(data[2]);
	strcpy(response, "1");
}

void Game::Ini(const char* data[], char* response)
{
	// set color
	if(!strcmp(data[1], "R"))
	{
		state->color = RED;
	}
	else if(!strcmp(data[1], "B"))
	{
		state->color = BLUE;
	}

	char position[15];
	state->Init_board_state(position);

	sprintf(response, "%c%c %c%c %c%c %c%c %c%c %c%c", position[0], position[1], position[2], position[3],
													   position[4], position[5], position[6], position[7],
													   position[8], position[9], position[10], position[11]);

}

void Game::Get(const char* data[], char* response)
{
	// set color
	if(!strcmp(data[1], "R"))
	{
		state->color = RED;
	}
	else if(!strcmp(data[1], "B"))
	{
		state->color = BLUE;
	}

	// set dice & board
	state->dice = stoi(data[2]);
	char position[25];
	sprintf(position, "%s%s%s%s%s%s%s%s%s%s%s%s", data[3], data[4], data[5], data[6], data[7], data[8], 
												  data[9], data[10], data[11], data[12], data[13], data[14]);
	state->Set_board(position);

	// generate move
	char move[4];
	NegaScout* AI = new NegaScout();
	Move cube_move(-1, -1, -1);
	AI->add_node(state, cube_move, -1, 0);
	// AI->Generate_random_move(move);
	AI->Generate_move(move);
	sprintf(response, "%c%c %c%c", move[0], move[1], move[2], move[3]);
	delete AI;
}

void Game::Exit(const char* data[], char* response)
{
	fprintf(stderr, "Bye~\n");
}