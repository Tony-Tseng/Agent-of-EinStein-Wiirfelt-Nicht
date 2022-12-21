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

float NegaScout::Star0_search(Board* b, float alpha, float beta, int depth){
	float p[6];
	b->cal_probability(p, b->color);
	float p_sum = p[0] + p[1] + p[2] + p[3] + p[4] + p[5];
	if(p_sum==0) return 0.0;

	float v_sum = 0.0;
	int turn = b->color;
	for(int i=0;i<6;i++){
		if( turn == RED && b->red_exist[i] ){
			b->dice = i+1;
			v_sum += p[i] * Search(b, alpha, beta, depth);
		}
		else if( turn == BLUE && b->blue_exist[i] ){
			b->dice = i+1;
			v_sum += p[i] * Search(b, alpha, beta, depth);
		}
	}

	return v_sum/ p_sum;
}

float NegaScout::Star1_search(Board* b, float alpha, float beta, int depth){
	float p[6];
	int turn = b->color;
	b->cal_probability(p, turn);
	float p_sum = p[0] + p[1] + p[2] + p[3] + p[4] + p[5];
	if(p_sum==0) return 0.0;

	// A_0 = ( (alpha - MAXVALUE) * p_sum + MAXVALUE * p[0] ) / p[0]
	// B_0 = ( (beta  - MINVALUE) * p_sum + MINVALUE * p[0] ) / p[0]
	float A, B;
	float A_num_next, B_num_next;
	float A_num = (alpha - MAXVALUE) * p_sum + MAXVALUE * p[0];
	float B_num = (beta  - MINVALUE) * p_sum + MINVALUE * p[0];
	float M = MAXVALUE;
	float m = MINVALUE;

	float v_sum = 0.0;
	float tmp = 0.0;
	for(int i=0;i<6;i++){
		if(i==0){
			A = A_num / p[0];
			B = B_num / p[0];
		}
		else{
			A = (p[i-1] * A_num_next - p[i] * A_num) / (p[i] * p[i-1]);
			B = (p[i-1] * B_num_next - p[i] * B_num) / (p[i] * p[i-1]);
			A_num = A_num_next;
			B_num = B_num_next;
		}

		if( turn == RED && b->red_exist[i] ){
			b->dice = i+1;
			tmp = p[i] * Search(b, std::max(alpha, A), std::min(beta, B), depth);
			m = m + p[i] / p_sum * (tmp-MINVALUE);
			M = M + p[i] / p_sum * (tmp-MAXVALUE);

			if(tmp >= B) return m;
			if(tmp <= A) return M;
			v_sum += tmp;
			A_num_next = A_num + (MAXVALUE - tmp) * p[i];
			B_num_next = B_num + (MINVALUE - tmp) * p[i];
		}
		else if( turn == BLUE && b->blue_exist[i] ){
			b->dice = i+1;
			tmp = p[i] * Search(b, std::max(alpha, A), std::min(beta, B), depth);
			m = m + p[i] / p_sum * (tmp-MINVALUE);
			M = M + p[i] / p_sum * (tmp-MAXVALUE);

			if(tmp >= B) return m;
			if(tmp <= A) return M;
			v_sum += tmp;
			A_num_next = A_num + (MAXVALUE - tmp) * p[i];
			B_num_next = B_num + (MINVALUE - tmp) * p[i];
		}
	}

	return v_sum/ p_sum;
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

	float p[6], p_sum;
	int target[2] = {0, 24};
	float score = 0.0;
	float final_score = 0.0;
	for(int i=0;i<2;i++){
		b->cal_probability(p, 1-i);
		p_sum = p[0] + p[1] + p[2] + p[3] + p[4] + p[5];
		score = 0.0;
		for(int j=0;j<6;j++){
			if(b->cube_position[i*6+j] == -1) continue;
			int distance = abs(b->cube_position[i*6+j] - target[i]);
			// score = std::max( score, (8 - (distance/5 + distance%5) ) * p[j] );
			score += (8.0 - (distance/5 + distance%5)) * p[j];
		}
		final_score += i == 0 ? score/p_sum : -score/p_sum;
	}

	if ( b->get_winner() == BLUE ) final_score += 24;
	else if( b->get_winner() == RED ) final_score -= 24;

	if(color == BLUE) return -final_score;
	else return final_score;
}