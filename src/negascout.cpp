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
	Board* traverse = new Board();
	*traverse = *tree_node[0].state;
	int result[100];
	int move_count = tree_node[0].state->get_legal_move(result);
	float alpha = -100;
	float beta = 100;
	float val = -100; // m
	float n = beta; // n

	for(int i=0;i<move_count;i++){
		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);

		// int cur_index = head_index;
		// add_node(traverse, cube_move, 0, 1);

		#ifdef V0
		float score = Star0_search(traverse, -n, -std::max(alpha, val), search_depth-1); // negascout
		#endif
		#ifdef V1
		float score = Star1_search(traverse, -n, -std::max(alpha, val), search_depth-1); // negascout
		#endif

		if(score>val){
			if(n==beta || score>=beta){
				val = score;
			}
			#ifdef V0
			else val = Star0_search(traverse, -beta, -score, search_depth-1);
			#endif
			#ifdef V1
			else val = Star1_search(traverse, -beta, -score, search_depth-1);
			#endif
			result_index = i;
		}
		*traverse = *tree_node[0].state;

		if(val>=beta) break;
		
		n = std::max(alpha, val) + 1;
	}

	// Result index should iterate child to get the best move
	// Move cube_move = tree_node[result_index].cube_move;
	tree_node[0].state->Output_move(move, result[result_index*3], result[result_index*3+1], result[result_index*3+2]);
}

void NegaScout::add_node(Board* b, Move cube_move, int p_id, int depth){
	tree_node[head_index].set_node(b, cube_move, p_id, depth);
	head_index++;
}

float NegaScout::Star1_search(Board* b, float alpha, float beta, int depth){
	int turn = b->color;
	float total = 6.0;

	float A = (alpha - MAXVALUE) * total + MAXVALUE;
	float B = (beta  - MINVALUE) * total + MINVALUE;
	float M = MAXVALUE;
	float m = MINVALUE;

	float v_sum = 0.0;
	float tmp = 0.0;
	for(int i=0;i<6;i++){
		if( turn == RED ){
			b->dice = i+1;
			tmp = -Search(b, std::max( (float) MINVALUE, A), std::min( (float) MAXVALUE, B), depth);
			m = m + (tmp-MINVALUE) / total;
			M = M + (tmp-MAXVALUE) / total;

			if(tmp >= B) return m;
			if(tmp <= A) return M;
			// if(m >= beta) return m;
			// if(M <= alpha) return M;
			v_sum += tmp;

			A = A - tmp + MAXVALUE;
			B = B - tmp + MINVALUE;
		}
		else if( turn == BLUE ){
			b->dice = i+1;
			tmp = -Search(b, std::max( (float) MINVALUE, A), std::min( (float) MAXVALUE, B), depth);
			m = m + (tmp-MINVALUE) / total;
			M = M + (tmp-MAXVALUE) / total;

			if(tmp >= B) return m;
			if(tmp <= A) return M;
			// if(m >= beta) return m;
			// if(M <= alpha) return M;
			v_sum += tmp;
			A = A - tmp + MAXVALUE;
			B = B - tmp + MINVALUE;
		}
	}

	return -v_sum/ total;
}

float NegaScout::Search(Board* b, float alpha, float beta, int depth){
	Board* traverse = new Board();
	*traverse = *b;

	int result[100];
	int move_count = traverse->get_legal_move(result);
	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
		return evaluate(b, b->color);
	}
	float val = -100; // m
	float n = beta; // n

	for(int i=0;i<move_count;i++){
		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		// add_node(traverse, cube_move, 0, 1);

		#ifdef V0
		float score = Star0_search(traverse, -n, -std::max(alpha, val), depth-1); // negascout
		#endif
		#ifdef V1
		float score = Star1_search(traverse, -n, -std::max(alpha, val), depth-1); // negascout
		#endif
		// b->Print_chessboard();

		if(score>val){
			if(n==beta || score>=beta || depth<3){
				val = score;
			}
			#ifdef V0
			else val = Star0_search(traverse, -beta, -score, depth-1);
			#endif
			#ifdef V1
			else val = Star1_search(traverse, -beta, -score, depth-1);
			#endif
		}
		*traverse = *b;

		if(val>=beta) break;
		
		n = std::max(alpha, val) + 1;
	}
	return -val;
}

float NegaScout::evaluate(Board* b, int color){
	// b->Print_chessboard();
	// float p[6];
	int target[2] = {0, 24};
	float score = 0.0;
	float final_score = 0.0;
	for(int i=0;i<2;i++){
		// b->cal_probability(p, 1-i);
		score = 0.0;
		for(int j=0;j<6;j++){
			if(b->cube_position[i*6+j] == -1) continue;
			int distance = abs(b->cube_position[i*6+j] - target[i]);
			score = std::max( score, (float) (8 - (distance/5 + distance%5)) );
		}
		final_score += i == 0 ? score: -score;
	}

	if(color == BLUE) return -final_score;
	else return final_score;
}