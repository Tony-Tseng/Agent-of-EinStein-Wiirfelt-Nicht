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

	for(int i=0;i<move_count;i++){
		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		// add_node(traverse, cube_move, 0, 1);

		float score = Star0_F(traverse, std::max(alpha, val), beta, search_depth-1); // negascout
		// b->Print_chessboard();

		if(score>val){
			val = score;
			result_index = i;
		}
		*traverse = *tree_node[0].state;

		if(val>=beta) break;
	}

	// Result index should iterate child to get the best move
	// Move cube_move = tree_node[result_index].cube_move;
	tree_node[0].state->Output_move(move, result[result_index*3], result[result_index*3+1], result[result_index*3+2]);
}

void NegaScout::add_node(Board* b, Move cube_move, int p_id, int depth){
	tree_node[head_index].set_node(b, cube_move, p_id, depth);
	head_index++;
}

float NegaScout::Star0_F(Board* b, float alpha, float beta, int depth){
	float A = (alpha - MAXVALUE) * 6.0 + MAXVALUE;
	float B = (beta  - MINVALUE) * 6.0 + MINVALUE;
	float M = MAXVALUE;
	float m = MINVALUE;
	float v_sum = 0.0;
	float tmp = 0.0;

	for(int i=0;i<6;i++){
		b->dice = i+1;
		tmp = Search_G(b, std::max(A, (float)MINVALUE), std::min(B, (float)MAXVALUE), depth);
		// tmp = Search_G(b, MINVALUE, MAXVALUE, depth);

		m = m + (tmp-MINVALUE) / 6.0;
		M = M + (tmp-MAXVALUE) / 6.0;

		// if(m >=beta) return m;
		// if(M <=alpha) return M;

		if(tmp >= B) return m;
		if(tmp <= A) return M;

		v_sum += tmp;
		A = A + MAXVALUE -tmp;
		B = B + MINVALUE -tmp;
	}

	return v_sum/6;
}

float NegaScout::Star0_G(Board* b, float alpha, float beta, int depth){
	float A = (alpha - MAXVALUE) * 6.0 + MAXVALUE;
	float B = (beta  - MINVALUE) * 6.0 + MINVALUE;
	float M = MAXVALUE;
	float m = MINVALUE;
	float v_sum = 0.0;
	float tmp = 0.0;

	for(int i=0;i<6;i++){
		b->dice = i+1;
		tmp = Search_F(b, std::max(A, (float)MINVALUE), std::min(B, (float)MAXVALUE), depth);
		// tmp = Search_F(b, MINVALUE, MAXVALUE, depth);

		m = m + (tmp-MINVALUE) / 6.0;
		M = M + (tmp-MAXVALUE) / 6.0;

		// if(m >=beta) return m;
		// if(M <=alpha) return M;

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

	int result[100];
	int move_count = traverse->get_legal_move(result);
	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
		return evaluate(b, b->color);
	}

	float val = -100; // m
	int i=0;
	Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
	traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);

	val = std::max(val, Star0_F(traverse, alpha, beta, depth-1));
	*traverse = *b;
	if(val>=beta) return val;

	for(i=1;i<move_count;i++){
		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		// add_node(traverse, cube_move, 0, 1);

		float score = Star0_F(traverse, val, val+1, depth-1); // negascout
		// b->Print_chessboard();

		if(score>val){
			if( score>=beta ){
				val = score;
			}
			else val = Star0_F(traverse, score, beta, depth-1);
		}
		*traverse = *b;

		if(val>=beta) break;
	}
	return val;
}


float NegaScout::Search_G(Board* b, float alpha, float beta, int depth){
	Board* traverse = new Board();
	*traverse = *b;

	int result[100];
	int move_count = traverse->get_legal_move(result);
	if(depth==0 || move_count == 0 || b->is_game_over() ){ // time limit
		return evaluate(b, b->color);
	}

	float val = 100; // m
	int i=0;
	Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
	traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);

	val = std::min(val, Star0_G(traverse, alpha, beta, depth-1));
	*traverse = *b;
	if(val<=alpha) return val;

	for(i=1;i<move_count;i++){
		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
		// add_node(traverse, cube_move, 0, 1);

		float score = Star0_G(traverse, val-1, val, depth-1); // negascout
		// b->Print_chessboard();

		if(score<val){
			if( score<=alpha ){
				val = score;
			}
			else val = Star0_G(traverse, alpha, score, depth-1);
		}
		*traverse = *b;

		if(val<=alpha) break;
	}
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
		final_score += i == tree_node[0].state->color ? -score: score;
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
// 	for(int i=0;i<move_count;i++){
// 		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
// 		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
// 		// add_node(traverse, cube_move, 0, 1);

// 		float score = Star0_F(traverse, std::max(alpha, val), beta, depth-1); // negascout
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
// 		Move cube_move = Move(result[i*3], result[i*3+1], result[i*3+2]);
// 		traverse->Make_move(result[i*3], result[i*3+1], result[i*3+2]);
// 		// add_node(traverse, cube_move, 0, 1);

// 		float score = Star0_G(traverse, alpha, std::min(beta, val), depth-1); // negascout
// 		// b->Print_chessboard();

// 		if(score<val){
// 			val = score;
// 		}
// 		*traverse = *b;

// 		if(val<=alpha) break;
// 	}
// 	return val;
// }