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
	float val = -1000000; // m
	float n = beta; // n

	// Print chessboard
	traverse->Print_chessboard();

	for(int i=0;i<move_count;i++){
		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		
		// Print chessboard
		traverse->Print_chessboard();

		int cur_index = head_index;
		add_node(traverse, cube_move, 0, 1);
		float score = Star0_search(traverse, -n, -std::max(alpha, val), search_depth-1); // negascout

		if(score>val){
			if(n==beta || score>=beta){
				val = score;
				result_index = cur_index;
			}
			else val = Star0_search(traverse, -beta, -score, search_depth-1);
		}
		traverse = tree_node[0].state;

		// Print chessboard
		traverse->Print_chessboard();

		if(val>=beta) break;
		
		n = std::max(alpha, val) + 1;
	}

	Move cube_move = tree_node[result_index].cube_move;
	tree_node[result_index].state->Output_move(move, cube_move.piece, cube_move.start_point, cube_move.end_point);
}

void NegaScout::add_node(Board* b, Move cube_move, int p_id, int depth){
	tree_node[head_index].set_node(b, cube_move, p_id, depth);
	head_index++;
}

float NegaScout::Star0_search(Board* b, float alpha, float beta, int depth){
	float p[6];
	b->cal_probability(p, b->color);
	float p_sum = p[0] + p[1] + p[2] + p[3] + p[4] + p[5];

	float v_sum = 0.0;
	int turn = b->color;
	for(int i=0;i<6;i++){
		if(turn == RED && b->red_exist[i]){
			b->dice = i+1;
			v_sum += p[i] * Search(b, alpha, beta, depth);
		}
		else if(turn == BLUE && b->blue_exist[i]){
			b->dice = i+1;
			v_sum += p[i] * Search(b, alpha, beta, depth);
		}
	}

	return v_sum/ p_sum;
}

float NegaScout::Search(Board* b, float alpha, float beta, int depth){
	Board* traverse = new Board();
	*traverse = *b;

	// Print chessboard
	// traverse->Print_chessboard();

	int result[100];
	int move_count = traverse->get_legal_move(result);
	if(depth==0 || move_count == 0){ // time limit
		return evaluate(b, b->color);
	}
	float val = -1000000; // m
	float n = beta; // n

	for(int i=0;i<move_count;i++){
		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		add_node(traverse, cube_move, 0, 1);

		// Print chessboard
		traverse->Print_chessboard();

		float score = Star0_search(traverse, -n, -std::max(alpha, val), depth-1); // negascout

		if(score>val){
			if(n==beta || score>=beta || depth<3){
				val = score;
			}
			else val = Star0_search(traverse, -beta, -score, depth-1);
		}
		*traverse = *b;

		// Print chessboard
		traverse->Print_chessboard();

		if(val>=beta) break;
		
		n = std::max(alpha, val) + 1;
	}
	return -val;
}

float NegaScout::evaluate(Board* b, int color){
	// float p[6];
	// b->Print_chessboard();

	int target[2] = {0, 24};
	float score = 0.0;
	for(int i=0;i<2;i++){
		// b->cal_probability(p, i);
		for(int j=0;j<6;j++){
			if(b->cube_position[i*6+j] == -1) continue;
			int distance = b->cube_position[i*6+j] - target[i];
			float manhattan = 8.0 - (abs(distance/5) + abs(distance%5));
			manhattan = i == color ? -manhattan : manhattan;
			score += manhattan;
		}
	}

	return -score;
}