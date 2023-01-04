#include "strategy.hpp"

float Manhattan::Evaluate_nearest(Board* b, int color, int* cube_piece) {
	int target[2] = {0, 24};
	float score = 0.0;
	float final_score = 0.0;
	for(int i=0;i<2;i++){
		if( cube_piece[i] == -1) score = 0;
		else{
			int distance = abs(b->cube_position[cube_piece[i]] - target[i]);
			score = (float) (8.0 - (distance/5 + distance%5) );
		}
		final_score += i == color ? -score: score;
	}
	return final_score;
}

float CubeStep::Evaluate_nearest(Board* b, int color, int* cube_piece) {
	int target[2] = {0, 24};
	float final_score = 0.0;
	int distance = 0;
	if(cube_piece[0] != -1 ){
		distance = abs(b->cube_position[cube_piece[0]] - target[0]);
		final_score += (4 - std::max(distance/5, distance%5));
	}

	if(cube_piece[1] != -1 ){
		distance = abs(b->cube_position[cube_piece[1]] - target[1]);
		final_score -= (4 - std::max(distance/5, distance%5));
	}
	return color == BLUE? final_score: -final_score;
}

float Prob::Evaluate_nearest(Board* b, int color, int* cube_piece) {
	float p[6];
	int index = 0;
	float final_score = 0.0;
	b->cal_probability(p, BLUE);
	if(cube_piece[0] != -1 ){
		index = cube_piece[0] % 6;
		final_score += p[index];
	}
	b->cal_probability(p, RED);
	if(cube_piece[1] != -1 ){
		index = cube_piece[1] % 6;
		final_score -= p[index];
	}
	return color == BLUE? final_score: -final_score;
}

// Blue
float Piece::Evaluate_nearest(Board* b, int color, int* cube_piece){
	float final_score = 0.0;

	float p[6];
	if( b->blue_piece_num <= 3 && b->blue_piece_num > 1){ // && b->red_piece_num - b->blue_piece_num < 2
		b->cal_probability(p, 1);
		if(p[0] + p[1] + p[2] + p[3] + p[4] + p[5] <= 6) final_score -= 1;
	}

	if( b->red_piece_num <= 3 && b->red_piece_num > 1){ // && b->blue_piece_num - b->red_piece_num < 2
		b->cal_probability(p, 0);
		if(p[0] + p[1] + p[2] + p[3] + p[4] + p[5] <= 6) final_score += 1;
	}
	return color == BLUE? final_score: -final_score;
}

float Corner::Evaluate_nearest(Board* b, int color, int* cube_piece){
	float final_score = 0.0;

	for(int i=0;i<6;i++){
		if( b->cube_position[i] == 24 || b->cube_position[i] == 23 || b->cube_position[i] == 19 ){
			final_score += 1;
		}
	}
	for(int i=6;i<12;i++){
		if( b->cube_position[i] == 0 || b->cube_position[i] == 1 || b->cube_position[i] == 5 ){
			final_score -= 1;
		}
	}
	return color == BLUE? final_score: -final_score; 
}

float Threat::Evaluate_nearest(Board* b, int color, int* cube_piece){
	float final_score = 0.0;

	int blue_pos = cube_piece[0];
	for(int i=6;i<12;i++){
		if( b->cube_position[i] == -1 ) continue;
		if( b->cube_position[i] + 1 == blue_pos || b->cube_position[i] + 5 == blue_pos || b->cube_position[i] + 6 == blue_pos ){
			final_score -= 1;
		}
	}
	int red_pos = cube_piece[1];
	for(int i=0;i<6;i++){
		if( b->cube_position[i] == -1 ) continue;
		if( b->cube_position[i] - 1 == red_pos || b->cube_position[i] - 5 == red_pos || b->cube_position[i] - 6 == red_pos ){
			final_score += 1;
		}
	}

	return color == BLUE? -final_score: final_score; 
}

float Protection::Evaluate_nearest(Board* b, int color, int* cube_piece){
	float final_score = 0.0;

	int blue_pos = cube_piece[0];
	for(int i=0;i<6;i++){
		if( b->cube_position[i] == blue_pos || b->cube_position[i] == -1 ) continue;
		if( b->cube_position[i] + 1 == blue_pos || b->cube_position[i] + 5 == blue_pos || b->cube_position[i] + 6 == blue_pos ){
			final_score += 1;
		}
	}
	int red_pos = cube_piece[1];
	for(int i=6;i<12;i++){
		if( b->cube_position[i] == blue_pos || b->cube_position[i] == -1 ) continue;
		if( b->cube_position[i] - 1 == red_pos || b->cube_position[i] - 5 == red_pos || b->cube_position[i] - 6 == red_pos ){
			final_score -= 1;
		}
	}

	return color == BLUE? final_score: -final_score; 
}

// Blue
float Turn::Evaluate_nearest(Board* b, int color, int* cube_piece){
	return color == b->color? -1: 1;
}