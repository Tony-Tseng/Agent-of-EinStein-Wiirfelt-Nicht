#include "negascout.hpp"

void NegaScout::Generate_random_move(char* move){
	int result[100];
	// get legal moves
	int move_count = root->get_legal_move(result);
	// randomly choose a legal move
	int rand_move = rand() % move_count;
	int piece = result[rand_move * 3];
	int start_point = result[rand_move * 3 + 1];
	int end_point = result[rand_move * 3 + 2];

	root->Output_move(move, piece, start_point, end_point);
}

void NegaScout::Generate_move(char* move){
	int result_index = 0;

	// NegaScout Algorithm
	Board* traverse = new Board();
	*traverse = *root;
	int next_hash = 0;
	int result[100];
	int move_count = root->get_legal_move(result);
	float alpha = -100;
	float beta = 100;
	float val = -100; // m

	for(int i=0;i<move_count;i++){
		next_hash = transposition_table->Calculate_hash_by_move(traverse, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->hash_value = next_hash;

		float score = Star1_F(traverse, std::max(alpha, val), beta, search_depth-1); // negascout

		if(score>val){
			val = score;
			result_index = i;
		}
		*traverse = *root;

		if(val>=beta) break;
	}
	// Result index should iterate child to get the best move
	root->Output_move(move, result[result_index*3], result[result_index*3+1], result[result_index*3+2]);
}

float NegaScout::Star1_F(Board* b, float alpha, float beta, int depth){
	float A = (alpha - MAXVALUE) * 6.0 + MAXVALUE;
	float B = (beta  - MINVALUE) * 6.0 + MINVALUE;
	float M = MAXVALUE;
	float m = MINVALUE;
	float v_sum = 0.0;
	float tmp = 0.0;

	for(int i=0;i<6;i++){
		b->dice = i+1;
		tmp = Search_G(b, std::max(A, (float)MINVALUE), std::min(B, (float)MAXVALUE), depth);

		m = m + (tmp-MINVALUE) / 6.0;
		M = M + (tmp-MAXVALUE) / 6.0;

		if(tmp >= B) return m;
		if(tmp <= A) return M;

		v_sum += tmp;
		A = A + MAXVALUE -tmp;
		B = B + MINVALUE -tmp;
	}

	return v_sum/6;
}

float NegaScout::Star1_G(Board* b, float alpha, float beta, int depth){
	float A = (alpha - MAXVALUE) * 6.0 + MAXVALUE;
	float B = (beta  - MINVALUE) * 6.0 + MINVALUE;
	float M = MAXVALUE;
	float m = MINVALUE;
	float v_sum = 0.0;
	float tmp = 0.0;

	for(int i=0;i<6;i++){
		b->dice = i+1;
		tmp = Search_F(b, std::max(A, (float)MINVALUE), std::min(B, (float)MAXVALUE), depth);

		m = m + (tmp-MINVALUE) / 6.0;
		M = M + (tmp-MAXVALUE) / 6.0;

		if(tmp >= B) return m;
		if(tmp <= A) return M;

		v_sum += tmp;
		A = A + MAXVALUE -tmp;
		B = B + MINVALUE -tmp;
	}

	return v_sum/6;
}

float NegaScout::Search_F(Board* b, float alpha, float beta, int depth){
	Board* traverse = new Board();
	*traverse = *b;

	// Trasposition Table Check
	// int table_index = transposition_table->Get_board_index(traverse);
	// Table_Entry entry = transposition_table->table[table_index];
	// if( entry.occupied && entry.depth >= depth ){
	// 	bool check_same = transposition_table->Check_same_state(traverse, table_index);
	// 	if(check_same){
	// 		if(entry.value_type == EXACT_VALUE) 
	// 			return entry.best_value;
	// 		if (entry.value_type == LOWER_BOUND){
	// 			alpha = std::max(alpha, entry.best_value);
	// 			if(alpha >=beta) return alpha;
	// 		} 
	// 		else if (entry.value_type == UPPER_BOUND){
	// 			beta = std::min(beta, entry.best_value);
	// 			if(alpha >=beta) return beta;
	// 		} 
	// 	}
	// }
	// End Table Check
	int best_index = 0;

	int result[100];
	int move_count = traverse->get_legal_move(result);
	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
		return evaluate(b, b->color);
	}

	float val = -100; // m
	int i=0;
	int next_hash = 0;
	// next_hash = transposition_table->Calculate_hash_by_move(traverse, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
	traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
	// traverse->hash_value = next_hash;

	val = std::max(val, Star1_F(traverse, alpha, beta, depth-1));
	*traverse = *b;
	if(val>=beta) return val;

	for(i=1;i<move_count;i++){
		// next_hash = transposition_table->Calculate_hash_by_move(traverse, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		// traverse->hash_value = next_hash;

		float score = Star1_F(traverse, val, val+1, depth-1); // negascout

		if(score>val){
			if( score>=beta ){
				val = score;
			}
			else val = Star1_F(traverse, score, beta, depth-1);
			best_index = i;
		}
		if(val>=beta){
			// transposition_table->Set_entry(traverse, depth, val, LOWER_BOUND, result, best_index);
			return val;
		}
		*traverse = *b;
	}
	// next_hash = transposition_table->Calculate_hash_by_move(traverse, traverse->hash_value, result[best_index*3], result[best_index*3+1], result[best_index*3+2]);
	// traverse->Make_move(result[best_index*3], result[best_index*3+1], result[best_index*3+2]);
	// traverse->hash_value = next_hash;
	
	// if( val > alpha) transposition_table->Set_entry(traverse, depth, val, EXACT_VALUE, result, best_index);
	// else transposition_table->Set_entry(traverse, depth, val, UPPER_BOUND, result, best_index);

	return val;
}


float NegaScout::Search_G(Board* b, float alpha, float beta, int depth){
	Board* traverse = new Board();
	*traverse = *b;

	// Trasposition Table Check
	// int table_index = transposition_table->Get_board_index(traverse);
	// Table_Entry entry = transposition_table->table[table_index];
	// if( entry.occupied && entry.depth >= depth ){
	// 	bool check_same = transposition_table->Check_same_state(traverse, table_index);
	// 	if(check_same){
	// 		if(entry.value_type == EXACT_VALUE) 
	// 			return entry.best_value;
	// 		if(entry.value_type == LOWER_BOUND){
	// 			alpha = std::max(alpha, entry.best_value);
	// 			if(alpha >=beta) return alpha;
	// 		} 
	// 		else if (entry.value_type == UPPER_BOUND){
	// 			beta = std::min(beta, entry.best_value);
	// 			if(alpha >=beta) return beta;
	// 		} 
	// 	}
	// }
	// End Table Check
	int best_index = 0;

	int result[100];
	int move_count = traverse->get_legal_move(result);
	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
		return evaluate(b, b->color);
	}

	float val = 100; // m
	int i=0;
	int next_hash = 0;
	// next_hash = transposition_table->Calculate_hash_by_move(traverse, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
	traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
	// traverse->hash_value = next_hash;

	val = std::min(val, Star1_G(traverse, alpha, beta, depth-1));
	*traverse = *b;
	if(val<=alpha) return val;

	for(i=1;i<move_count;i++){
		// next_hash = transposition_table->Calculate_hash_by_move(traverse, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		// traverse->hash_value = next_hash;

		float score = Star1_G(traverse, val-1, val, depth-1); // negascout

		if(score<val){
			if( score<=alpha ){
				val = score;
			}
			else val = Star1_G(traverse, alpha, score, depth-1);
			best_index = i;
		}
		if(val<=alpha){
			// transposition_table->Set_entry(traverse, depth, val, UPPER_BOUND, result, best_index);
			return val;
		}
		*traverse = *b;
	}
	// next_hash = transposition_table->Calculate_hash_by_move(traverse, traverse->hash_value, result[best_index*3], result[best_index*3+1], result[best_index*3+2]);
	// traverse->Make_move(result[best_index*3], result[best_index*3+1], result[best_index*3+2]);
	// traverse->hash_value = next_hash;
	
	// if( val < beta) transposition_table->Set_entry(traverse, depth, val, EXACT_VALUE, result, best_index);
	// else transposition_table->Set_entry(traverse, depth, val, LOWER_BOUND, result, best_index);
	return val;
}

float NegaScout::evaluate(Board* b, int color){
	int target[2] = {0, 24};
	float score = 0.0;
	float final_score = 0.0;
	for(int i=0;i<2;i++){
		score = 0.0;
		for(int j=0;j<6;j++){
			if(b->cube_position[i*6+j] == -1) continue;
			int distance = abs(b->cube_position[i*6+j] - target[i]);
			score = std::max( score, (float) (8.0 - (distance/5 + distance%5) ) );
		}
		final_score += i == 0 ? -score: score;
	}
	return final_score;
}

// float NegaScout::Search_F(Board* b, float alpha, float beta, int depth){
// 	Board* traverse = new Board();
// 	*traverse = *b;

// 	int result[100];
// 	int move_count = traverse->get_legal_move(result);
// 	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
// 		return evaluate(b, b->color);
// 	}

// 	float val = -100; // m
// 	int i=0;
// 	traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);

// 	val = std::max(val, Star1_F(traverse, alpha, beta, depth-1));
// 	*traverse = *b;
// 	if(val>=beta) return val;

// 	for(i=1;i<move_count;i++){
// 		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);

// 		float score = Star1_F(traverse, val, val+1, depth-1); // negascout

// 		if(score>val){
// 			if( score>=beta || depth<3){
// 				val = score;
// 			}
// 			else val = Star1_F(traverse, score, beta, depth-1);
// 		}
// 		*traverse = *b;

// 		if(val>=beta) break;
// 	}
// 	return val;
// }


// float NegaScout::Search_G(Board* b, float alpha, float beta, int depth){
// 	Board* traverse = new Board();
// 	*traverse = *b;

// 	int result[100];
// 	int move_count = traverse->get_legal_move(result);
// 	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
// 		return evaluate(b, b->color);
// 	}

// 	float val = 100; // m
// 	int i=0;
// 	traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);

// 	val = std::min(val, Star1_G(traverse, alpha, beta, depth-1));
// 	*traverse = *b;
// 	if(val<=alpha) return val;

// 	for(i=1;i<move_count;i++){
// 		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);

// 		float score = Star1_G(traverse, val-1, val, depth-1); // negascout

// 		if(score<val){
// 			if( score<=alpha || depth<3){
// 				val = score;
// 			}
// 			else val = Star1_G(traverse, alpha, score, depth-1);
// 		}
// 		*traverse = *b;

// 		if(val<=alpha) break;
// 	}
// 	return val;
// }


// float NegaScout::Search_F(Board* b, float alpha, float beta, int depth){
// 	Board* traverse = new Board();
// 	*traverse = *b;

// 	int result[100];
// 	int move_count = traverse->get_legal_move(result);
// 	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
// 		return evaluate(b, b->color);
// 	}

// 	float val = -100; // m
// 	for(int i=0;i<move_count;i++){
// 		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);

// 		float score = Star1_F(traverse, std::max(alpha, val), beta, depth-1); // negascout
// 		// b->Print_chessboard();

// 		if(score>val){
// 			val = score;
// 		}
// 		*traverse = *b;

// 		if(val>=beta) break;
// 	}
// 	return val;
// }


// float NegaScout::Search_G(Board* b, float alpha, float beta, int depth){
// 	Board* traverse = new Board();
// 	*traverse = *b;

// 	int result[100];
// 	int move_count = traverse->get_legal_move(result);
// 	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
// 		return evaluate(b, b->color);
// 	}

// 	float val = 100; // m

// 	for(int i=0;i<move_count;i++){
// 		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);

// 		float score = Star1_G(traverse, alpha, std::min(beta, val), depth-1); // negascout
// 		// b->Print_chessboard();

// 		if(score<val){
// 			val = score;
// 		}
// 		*traverse = *b;

// 		if(val<=alpha) break;
// 	}
// 	return val;
// }