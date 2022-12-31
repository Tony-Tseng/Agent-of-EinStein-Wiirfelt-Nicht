#include "negascout.hpp"

double NegaScout::timer(bool reset){
	if ( reset ) {
		clock_gettime(CLOCK_REALTIME, &tick);
		return 0.0;
	}
	clock_gettime(CLOCK_REALTIME, &tock);
	return (double) ((tock.tv_sec + tock.tv_nsec*1e-9)-(double) (tick.tv_sec + tick.tv_nsec*1e-9));
}

void NegaScout::set_time_limit(double time_limit){
	this->time_limit = time_limit;
}

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
	double total_time = 0.0;
	double prev_time = 0.0, curr_time = 0.0;
	float alpha = -100;
	float beta = 100;

	std::pair<int ,float> IDAS_result = First_F(alpha, beta, 3);
	prev_time = 0.3;
	total_time = timer(false);

	std::pair<int ,float> IDAS_tmp;
	int current_depth = 5;
	
	while( timer(false) < time_limit && current_depth <= depth_limit){
		IDAS_tmp = First_F(IDAS_result.second-threshold, IDAS_result.second+threshold, current_depth);
		if(IDAS_tmp.second <= IDAS_result.second-threshold){
			IDAS_tmp = First_F(alpha, IDAS_tmp.second, current_depth);
		}
		else if(IDAS_tmp.second >= IDAS_result.second+threshold){
			IDAS_tmp = First_F(IDAS_tmp.second, beta, current_depth);
		}

		curr_time = timer(false) - total_time;
		total_time = timer(false);
		
		// if(current_depth%2==1){
		IDAS_result = IDAS_tmp;
		// }
		// Need to determine whether the time is enough
		double time_left = time_limit - total_time;
		double time_times = curr_time / prev_time;

		// std::cout << curr_time << " " << prev_time << " " << total_time << std::endl;
		// std::cout << current_depth << " " << time_times * curr_time * 0.7 << " " << time_left << std::endl;
		
		if( (time_times * curr_time * 0.8 > time_left )){
			break;
		}
		prev_time = curr_time;
		
		current_depth++;
	}
	
	// Result index should iterate child to get the best move
	int result_index = IDAS_result.first;
	int result[100];
	root->get_legal_move(result);
	root->Output_move(move, result[result_index*3], result[result_index*3+1], result[result_index*3+2]);
}

std::pair<int, float> NegaScout::First_F(float alpha, float beta, int depth){
	int result_index = 0;

	// NegaScout Algorithm
	Board* traverse = new Board();
	*traverse = *root;
	float val = -100; // m

	int next_hash = 0;
	int result[100];
	int move_count = root->get_legal_move(result);
	if(move_count == 1){
		return std::make_pair(0, MAXVALUE);
	}

	for(int i=0;i<move_count;i++){
		next_hash = transposition_table->Calculate_hash_by_move(traverse, traverse->dice, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->hash_value = next_hash;

		float score = Star1_F(traverse, std::max(alpha, val), beta, depth-1); // negascout

		if(score>val){
			val = score;
			result_index = i;
		}
		*traverse = *root;

		if(val>=beta) break;
	}

	return std::make_pair(result_index, val);
}

float NegaScout::Star1_F(Board* b, float alpha, float beta, int depth){
	float A = (alpha - MAXVALUE) * 6.0 + MAXVALUE;
	float B = (beta  - MINVALUE) * 6.0 + MINVALUE;
	float M = MAXVALUE;
	float m = MINVALUE;
	float v_sum = 0.0;
	float tmp = 0.0;

	for(int i=0;i<6;i++){
		// b->dice = i+1;
		tmp = Search_G(b, i+1, std::max(A, (float)MINVALUE), std::min(B, (float)MAXVALUE), depth);

		m = m + (tmp-MINVALUE) / 6.0;
		M = M + (tmp-MAXVALUE) / 6.0;

		if(tmp >= B) return truncate(m, 5);
		if(tmp <= A) return truncate(M, 5);

		v_sum += tmp;
		A = A + MAXVALUE -tmp;
		B = B + MINVALUE -tmp;
	}

	return truncate(v_sum / 6.0, 5);
}

float NegaScout::Star1_G(Board* b, float alpha, float beta, int depth){
	float A = (alpha - MAXVALUE) * 6.0 + MAXVALUE;
	float B = (beta  - MINVALUE) * 6.0 + MINVALUE;
	float M = MAXVALUE;
	float m = MINVALUE;
	float v_sum = 0.0;
	float tmp = 0.0;

	for(int i=0;i<6;i++){
		// b->dice = i+1;
		tmp = Search_F(b, i+1, std::max(A, (float)MINVALUE), std::min(B, (float)MAXVALUE), depth);

		m = m + (tmp-MINVALUE) / 6.0;
		M = M + (tmp-MAXVALUE) / 6.0;

		if(tmp >= B) return truncate(m, 5);
		if(tmp <= A) return truncate(M, 5);

		v_sum += tmp;
		A = A + MAXVALUE -tmp;
		B = B + MINVALUE -tmp;
	}

	return truncate(v_sum / 6.0, 5);
}

float NegaScout::Search_F(Board* b, int next_dice, float alpha, float beta, int depth){
	Board* traverse = new Board();
	*traverse = *b;

	// Trasposition Table Check
	int table_index = transposition_table->Get_board_index(traverse);
	Table_Entry entry = transposition_table->table[table_index];
	if( entry.occupied && entry.depth >= depth ){
		bool check_same = transposition_table->Check_same_state(traverse, table_index);
		if(check_same){
			if(entry.value_type == EXACT_VALUE) 
				return entry.best_value;
			if (entry.value_type == LOWER_BOUND){
				alpha = std::max(alpha, entry.best_value);
				if(alpha >=beta) return alpha;
			} 
			else if (entry.value_type == UPPER_BOUND){
				beta = std::min(beta, entry.best_value);
				if(alpha >=beta) return beta;
			} 
		}
	}
	// End Table Check
	int best_index = 0;

	int result[100];
	traverse->dice = next_dice;
	int move_count = traverse->get_legal_move(result);
	*traverse = *b;
	if(depth==0 || move_count == 0 || b->is_game_over() ){
		return evaluate(b);
	}

	float val = -100; // m
	int i=0;
	int next_hash = 0;
	next_hash = transposition_table->Calculate_hash_by_move(traverse, next_dice, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
	traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
	traverse->hash_value = next_hash;

	val = std::max(val, Star1_F(traverse, alpha, beta, depth-1));
	*traverse = *b;
	if(val>=beta) return val;

	for(i=1;i<move_count;i++){
		next_hash = transposition_table->Calculate_hash_by_move(traverse, next_dice, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->hash_value = next_hash;

		float score = Star1_F(traverse, val, val+1, depth-1); // negascout

		if(score>val){
			if( score>=beta || depth<1 ){
				val = score;
			}
			else val = Star1_F(traverse, score, beta, depth-1);
			best_index = i;
		}
		if(val>=beta){
			transposition_table->Set_entry(traverse, depth, val, LOWER_BOUND, result, best_index);
			return val;
		}
		*traverse = *b;
	}
	next_hash = transposition_table->Calculate_hash_by_move(traverse, next_dice, traverse->hash_value, result[best_index*3], result[best_index*3+1], result[best_index*3+2]);
	traverse->Make_move(result[best_index*3], result[best_index*3+1], result[best_index*3+2]);
	traverse->hash_value = next_hash;
	
	if( val > alpha) transposition_table->Set_entry(traverse, depth, val, EXACT_VALUE, result, best_index);
	else transposition_table->Set_entry(traverse, depth, val, UPPER_BOUND, result, best_index);

	return val;
}

float NegaScout::Search_G(Board* b, int next_dice, float alpha, float beta, int depth){
	Board* traverse = new Board();
	*traverse = *b;

	// Trasposition Table Check
	int table_index = transposition_table->Get_board_index(traverse);
	Table_Entry entry = transposition_table->table[table_index];
	if( entry.occupied && entry.depth >= depth ){
		bool check_same = transposition_table->Check_same_state(traverse, table_index);
		if(check_same){
			if(entry.value_type == EXACT_VALUE) 
				return entry.best_value;
			if(entry.value_type == LOWER_BOUND){
				alpha = std::max(alpha, entry.best_value);
				if( alpha >= beta ) return alpha;
			} 
			else if (entry.value_type == UPPER_BOUND){
				beta = std::min(beta, entry.best_value);
				if( alpha >= beta ) return beta;
			} 
		}
	}
	// End Table Check
	int best_index = 0;

	int result[100];
	traverse->dice = next_dice;
	int move_count = traverse->get_legal_move(result);
	*traverse = *b;
	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
		return evaluate(b);
	}

	float val = 100; // m
	int i=0;
	int next_hash = 0;
	next_hash = transposition_table->Calculate_hash_by_move(traverse, next_dice, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
	traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
	traverse->hash_value = next_hash;

	val = std::min(val, Star1_G(traverse, alpha, beta, depth-1));
	*traverse = *b;
	if(val<=alpha) return val;

	for(i=1;i<move_count;i++){
		next_hash = transposition_table->Calculate_hash_by_move(traverse, next_dice, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->hash_value = next_hash;

		float score = Star1_G(traverse, val-1, val, depth-1); // negascout

		if(score<val){
			if( score<=alpha || depth<1 ){
				val = score;
			}
			else val = Star1_G(traverse, alpha, score, depth-1);
			best_index = i;
		}
		if(val<=alpha){
			transposition_table->Set_entry(traverse, depth, val, UPPER_BOUND, result, best_index);
			return val;
		}
		*traverse = *b;
	}
	next_hash = transposition_table->Calculate_hash_by_move(traverse, next_dice, traverse->hash_value, result[best_index*3], result[best_index*3+1], result[best_index*3+2]);
	traverse->Make_move(result[best_index*3], result[best_index*3+1], result[best_index*3+2]);
	traverse->hash_value = next_hash;
	
	if( val < beta) transposition_table->Set_entry(traverse, depth, val, EXACT_VALUE, result, best_index);
	else transposition_table->Set_entry(traverse, depth, val, LOWER_BOUND, result, best_index);
	return val;
}

float NegaScout::evaluate(Board* b){
	// if(b->is_game_over()){
	// 	if(b->red_piece_num == 0) return root->color == RED? MINVALUE: MAXVALUE;
	// 	else if(b->blue_piece_num == 0) return root->color == BLUE? MINVALUE: MAXVALUE;
	// }

	int cube_piece[2] = {-1, -1};
	Get_nearest(b, cube_piece);
	float final_score = 0.0;

	for(int i=0;i<num_strategy;i++){
		final_score += strategy[i]->Evaluate_nearest(b, root->color, cube_piece) * weight[i];
	}
	final_score += bias;

	return final_score;
}

void NegaScout::Get_nearest(Board* b, int* cube_piece){
	int target[2] = {0, 24};
	
	int min_manhattan = 8;
	for(int i=0;i<2;i++){
		min_manhattan = 8;
		for(int j=0;j<6;j++){
			if(b->cube_position[i*6+j] == -1) continue;
			int distance = abs(b->cube_position[i*6+j] - target[i]);
			int manhattan = distance/5 + distance%5;
			if(manhattan < min_manhattan){
				min_manhattan = manhattan;
				cube_piece[i] = i*6+j;
			}
		}
	}
}

float NegaScout::truncate(float value, int num){
	float temp = pow(10, num);
	value = value * temp;
	value = floor(value);
	return value/temp;
}

// void NegaScout::Generate_move(char* move){
// 	int result_index = 0;

// 	// NegaScout Algorithm
// 	Board* traverse = new Board();
// 	*traverse = *root;

// 	int next_hash = 0;
// 	int result[100];
// 	int move_count = root->get_legal_move(result);
// 	for(int i=0;i<move_count;i++){
// 		next_hash = transposition_table->Calculate_hash_by_move(traverse, traverse->hash_value, result[i*3], result[i*3+1], result[i*3+2]);
// 		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
// 		traverse->hash_value = next_hash;

		
// 	}

// 	// Result index should iterate child to get the best move
// 	root->Output_move(move, result[result_index*3], result[result_index*3+1], result[result_index*3+2]);
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