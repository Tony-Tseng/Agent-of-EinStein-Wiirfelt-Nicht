#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <string>

using std::stoi;
using std::string;

#define RED 0
#define BLUE 1
#define BOARD_SIZE 5
#define PIECE_NUM 6
#define COMMAND_NUM 7

class Board{
public:
	bool red_exist[PIECE_NUM], blue_exist[PIECE_NUM];
	int color;
	int red_time, blue_time;
	int board_size;
	int dice;
	int board[BOARD_SIZE][BOARD_SIZE];
	int red_piece_num, blue_piece_num;
	int cube_position[2*PIECE_NUM];

	int hash_value;

	Board();
	~Board();

	Board &operator=(const Board& C);
	
	// Board Related
	void Init_board_state(char* position);
	void Set_board(char* position);
	void Set_hash_value(int hash_value);
	void Print_chessboard();
	// void Generate_move(char* move);
	void Output_move(char* move, const int piece, const int start_point, const int end_point);
	void Make_move(const int piece, const int start_point, const int end_point);
	
	int get_legal_move(int* result); 
	int get_all_move(int *result);
	int referee(int piece, int* src, int* dst);

	void change_turn();
	void cal_probability(float* p, int color);
	bool is_game_over();
	int get_winner();
};