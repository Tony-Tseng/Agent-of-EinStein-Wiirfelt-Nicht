#include "negascout.hpp"

void NegaScout::Generate_random_move(char* move){
	int result[100];
	// get legal moves
	int move_count = tree_node[0].state->get_legal_move(result);
	// randomly choose a legal move
	int rand_move = rand() % move_count;
	int piece = result[rand_move * 3];
	int start_point = result[rand_move * 3 + 1];
	int end_point = result[rand_move * 3 + 2];

	tree_node[0].state->Output_move(move, piece, start_point, end_point);
}

void NegaScout::Generate_move(char* move){
	int result_index = 0;

	// NegaScout Algorithm
	Board* traverse = tree_node[0].state;
	int result[100];
	int move_count = tree_node[0].state->get_legal_move(result);
	float alpha = -1000000;
	float beta = 1000000;
	double val = alpha;
	for(int i=0;i<move_count;i++){
		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(cube_move);
		int cur_index = head_index;
		add_node(traverse, cube_move, 0, 1);
		float score = 0; // negascout

		if(score>val){
			val = score;
			result_index = cur_index;
		}
		traverse = tree_node[0].state;
		if(val>=beta){
			break;
		}
	}
	

	Move cube_move = tree_node[result_index].cube_move;
	tree_node[result_index].state->Output_move(move, cube_move.piece, cube_move.start_point, cube_move.end_point);
}

void NegaScout::add_node(Board* b, Move cube_move, int p_id, int depth){
	tree_node[head_index].set_node(b, cube_move, p_id, depth);
	head_index++;
}