#include "float.h"
#include "board.hpp"

Board::Board(void){}

Board::~Board(void){}

Board &Board::operator=(const Board& B){
	for(int i=0;i<PIECE_NUM;i++){
		red_exist[i] = B.red_exist[i];
		blue_exist[i] = B.blue_exist[i];
	}
	color = B.color;
	red_time = B.red_time;
	blue_time = B.blue_time;
	board_size = B.board_size;
	dice = B.dice;
	for(int i=0;i<BOARD_SIZE;i++){
		for(int j=0;j<BOARD_SIZE;j++){
			board[i][j] = B.board[i][j];
		}
	}
	red_piece_num = B.red_piece_num;
	blue_piece_num = B.blue_piece_num;
	for(int i=0;i<2*PIECE_NUM;i++){
		cube_position[i] = B.cube_position[i];
	}
	return (*this);
}

void Board::Init_board_state(char* position)
{
	int order[PIECE_NUM] = {0, 1, 2, 3, 4, 5};
	string red_init_position = "A1B1C1A2B2A3";
	string blue_init_position = "E3D4E4C5D5E5";

	// assign the initial positions of your pieces in random order
	for(int i = 0; i < PIECE_NUM; i++)
	{
		int j = rand() % (PIECE_NUM - i) + i;
		int tmp = order[i];
		order[i] = order[j];
		order[j] = tmp;
	}

	for(int i = 0; i < PIECE_NUM; i++)
	{
		if(this->color == RED)
		{
			position[order[i] * 2] = red_init_position[i * 2];
			position[order[i] * 2 + 1] = red_init_position[i * 2 + 1];
		}
		else if(this->color == BLUE)
		{
			position[order[i] * 2] = blue_init_position[i * 2];
			position[order[i] * 2 + 1] = blue_init_position[i * 2 + 1];
		}
	}
}

void Board::Set_board(char* position)
{
	memset(this->board, 0, sizeof(this->board));
	memset(this->cube_position, -1, sizeof(this->cube_position));
	memset(this->blue_exist, 1, sizeof(this->blue_exist));
	memset(this->red_exist, 1, sizeof(this->red_exist));
	this->blue_piece_num = PIECE_NUM;
	this->red_piece_num = PIECE_NUM;

	int lost_piece_num = 0;
	for(int i = 0; i < PIECE_NUM * 2; i++)
	{
		int index = i * 2 - lost_piece_num;

		// the piece does not exist
		while(position[index] == '0')
		{
			index = i * 2 - lost_piece_num + 1;
			lost_piece_num++;
			// blue
			if(i < PIECE_NUM)
			{
				this->blue_piece_num --;
				this->blue_exist[i] = 0;
			}
			// red
			else 
			{
				this->red_piece_num --;
				this->red_exist[i - PIECE_NUM] = 0;
			}
			i += 1;
		}
		// 1~6: blue pieces; 7~12: red pieces
		this->board[position[index + 1] - '1'][position[index] - 'A'] = i + 1;
		this->cube_position[i] = (position[index + 1] - '1') * BOARD_SIZE + (position[index] - 'A');
	}
	fprintf(stderr, "\nThe current board:\n");
	this->Print_chessboard();
}

void Board::Print_chessboard()
{
	fprintf(stderr, "\n");
	// 1~6 represent blue pieces; A~F represent red pieces
	for(int i = 0;i < BOARD_SIZE; i++)
	{
		fprintf(stderr, "<%d>   ", i + 1);
		for(int j = 0;j < BOARD_SIZE; j++)
		{
			if(this->board[i][j] <= PIECE_NUM) fprintf(stderr, "%d  ", this->board[i][j]);
			else fprintf(stderr, "%c  ", 'A' + (this->board[i][j] - 7)); 
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n     ");
	for (int i = 0;i < BOARD_SIZE; i++)
	{
		fprintf(stderr, "<%c>", 'A' + i);
	}
	fprintf(stderr, "\n\n");
	fprintf(stderr, "The number of blue pieces: %d\nThe number of red pieces: %d\n", this->blue_piece_num, this->red_piece_num);
}

void Board::Output_move(char* move, const int piece, const int start_point, const int end_point)
{
	sprintf(move, "%c%c%c%c", 'A' + start_point % BOARD_SIZE, '1' + start_point / BOARD_SIZE, 'A' + end_point % BOARD_SIZE, '1' + end_point / BOARD_SIZE);
	this->Make_move(piece, start_point, end_point);

	// print the result
	fprintf(stderr, "============================\nMy result:\n");
	if(piece <= PIECE_NUM) fprintf(stderr, "Blue piece %d: (%c%c) -> (%c%c)\n", piece, move[0], move[1], move[2], move[3]);
	else fprintf(stderr, "Red piece %d: (%c%c) -> (%c%c)\n", piece - PIECE_NUM, move[0], move[1], move[2], move[3]);
	this->Print_chessboard();
	fprintf(stderr, "============================\n");
}

// get all legal moves
int Board::get_legal_move(int* result)
{
	int src, dst[3];
	int movable_piece;
	int move_count = 0;
	int result_count = 0;

	if(this->color == BLUE)
	{
		// the corresponding piece is alive
		if(this->blue_exist[this->dice - 1])
		{
			movable_piece = this->dice;
			move_count = this->referee(movable_piece, &src, dst);
			for(int i = result_count; i < result_count + move_count; i++) 
			{
				result[i * 3] = movable_piece;
				result[i * 3 + 1] = src;
				result[i * 3 + 2] = dst[i];
			}
			result_count += move_count;
		}
		// the corresponding piece does not exist
		else
		{
			// seeking for the next-higher piece
			for(int i = this->dice; i <= PIECE_NUM; ++i)
			{
				if(this->blue_exist[i - 1])
				{
					movable_piece = i;
					move_count = this->referee(movable_piece, &src, dst);
					int index = 0;
					for(int j = result_count; j < result_count + move_count; j++, index++) 
					{
						result[j * 3] = movable_piece;
						result[j * 3 + 1] = src;
						result[j * 3 + 2] = dst[index];
					}
					result_count += move_count;
					break;
				}
			}
			// seeking for the next-lower piece
			for(int i = this->dice; i >= 1; --i)
			{
				if(this->blue_exist[i - 1])
				{
					movable_piece = i;
					move_count = this->referee(movable_piece, &src, dst);
					int index = 0;
					for(int j = result_count; j < result_count + move_count; j++, index++) 
					{
						result[j * 3] = movable_piece;
						result[j * 3 + 1] = src;
						result[j * 3 + 2] = dst[index];
					}
					result_count += move_count;
					break;
				}
			}
		}
	}

	else if(this->color == RED)
	{
		// the corresponding piece is alive
		if(this->red_exist[this->dice - 1])
		{
			movable_piece = this->dice + PIECE_NUM;
			move_count = this->referee(movable_piece, &src, dst);
			for(int i = result_count; i < result_count + move_count; i++) 
			{
				result[i * 3] = movable_piece;
				result[i * 3 + 1] = src;
				result[i * 3 + 2] = dst[i];
			}
			result_count += move_count;
		}
		// the corresponding piece does not exist
		else
		{
			// seeking for the next-higher piece
			for(int i = this->dice; i <= PIECE_NUM; ++i)
			{
				if(this->red_exist[i - 1])
				{
					movable_piece = i + PIECE_NUM; 
					move_count = this->referee(movable_piece, &src, dst);
					int index = 0;
					for(int j = result_count; j < result_count + move_count; j++, index++) 
					{
						result[j * 3] = movable_piece;
						result[j * 3 + 1] = src;
						result[j * 3 + 2] = dst[index];
					}
					result_count += move_count;
					break;
				}
			}
			// seeking for the next-lower piece
			for(int i = this->dice; i >= 1; --i)
			{
				if(this->red_exist[i - 1])
				{
					movable_piece = i + PIECE_NUM; 
					move_count = this->referee(movable_piece, &src, dst);
					int index = 0;
					for(int j = result_count; j < result_count + move_count; j++, index++) 
					{
						result[j * 3] = movable_piece;
						result[j * 3 + 1] = src;
						result[j * 3 + 2] = dst[index];
					}
					result_count += move_count;
					break;
				}
			}			
		}		
	}
	return result_count;
}

// get possible moves of the piece
int Board::referee(int piece, int* src, int* dst)
{
	// for(int i = 0; i < BOARD_SIZE; i++)
	// {
	// 	for(int j = 0; j < BOARD_SIZE; j++)
	// 	{
	// 		if(this->board[i][j] == piece)
	// 		{
	// 			*src = i * BOARD_SIZE + j;
	// 		}
	// 	}
	// }
	// Search by table
	*src = cube_position[piece-1];

	// blue piece
	if(piece <= PIECE_NUM) 
	{
		// the piece is on the leftmost column
		if(*src % BOARD_SIZE == 0)
		{
			dst[0] = *src - BOARD_SIZE; // up
			return 1;
		}
		// the piece is on the uppermost row
		else if(*src < BOARD_SIZE)
		{
			dst[0] = *src - 1; // left
			return 1;
		}
		else
		{
			dst[0] = *src - 1; // left
			dst[1] = *src - BOARD_SIZE; // up
			dst[2] = *src - BOARD_SIZE - 1; // upper left
			return 3;
		}
	}

	// red piece
	else
	{
		// the piece is on the rightmost column
		if(*src % BOARD_SIZE == 4)
		{
			dst[0] = *src + BOARD_SIZE; // down
			return 1;
		}
		// the piece is on the downmost row
		else if(*src >= BOARD_SIZE * (BOARD_SIZE - 1))
		{
			dst[0] = *src + 1; // right
			return 1;
		}
		else
		{
			dst[0] = *src + 1; // right
			dst[1] = *src + BOARD_SIZE; // down
			dst[2] = *src + BOARD_SIZE + 1; // bottom right
			return 3;
		}
	}	
}

void Board::Make_move(const int piece, const int start_point, const int end_point)
{
	int start_row = start_point / BOARD_SIZE;
	int start_col = start_point % BOARD_SIZE;
	int end_row = end_point / BOARD_SIZE;
	int end_col = end_point % BOARD_SIZE;

	this->board[start_row][start_col] = 0;

	// there has another piece on the target sqaure
	if(this->board[end_row][end_col] > 0)
	{
		if(this->board[end_row][end_col] <= PIECE_NUM)
		{
			cube_position[this->board[end_row][end_col] - 1] = -1;
			this->blue_exist[this->board[end_row][end_col] - 1] = 0;
			this->blue_piece_num--;
		}
		else
		{	
			cube_position[this->board[end_row][end_col] - 7] = -1;
			this->red_exist[this->board[end_row][end_col] - 7] = 0;
			this->red_piece_num--;			
		}
	}
	cube_position[piece-1] = this->board[end_row][end_col];
	this->board[end_row][end_col] = piece;
}

void Board::Make_move(Move cube_move)
{
	int start_point = cube_move.start_point;
	int end_point = cube_move.end_point;
	int piece = cube_move.piece;

	int start_row = start_point / BOARD_SIZE;
	int start_col = start_point % BOARD_SIZE;
	int end_row = end_point / BOARD_SIZE;
	int end_col = end_point % BOARD_SIZE;

	this->board[start_row][start_col] = 0;

	// there has another piece on the target sqaure
	if(this->board[end_row][end_col] > 0)
	{
		if(this->board[end_row][end_col] <= PIECE_NUM)
		{
			cube_position[this->board[end_row][end_col] - 1] = -1;
			this->blue_exist[this->board[end_row][end_col] - 1] = 0;
			this->blue_piece_num--;
		}
		else
		{	
			cube_position[this->board[end_row][end_col] - 7] = -1;
			this->red_exist[this->board[end_row][end_col] - 7] = 0;
			this->red_piece_num--;			
		}
	}
	cube_position[piece-1] = this->board[end_row][end_col];
	this->board[end_row][end_col] = piece;
}