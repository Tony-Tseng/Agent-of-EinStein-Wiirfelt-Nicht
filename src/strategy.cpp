#include "strategy.hpp"


// float Manhattan::Evaluate(Board* b, int color) {
// 	int target[2] = {0, 24};
// 	float score = 0.0;
// 	float final_score = 0.0;
// 	for(int i=0;i<2;i++){
// 		score = 0.0;
// 		for(int j=0;j<6;j++){
// 			if(b->cube_position[i*6+j] == -1) continue;
// 			int distance = abs(b->cube_position[i*6+j] - target[i]);
// 			score = std::max( score, (float) (8.0 - (distance/5 + distance%5) ) );
// 		}
// 		final_score += i == color ? -score: score;
// 	}
// 	return final_score;
// }

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
	float score = 0.0;
	float final_score = 0.0;
	for(int i=0;i<2;i++){
		if( cube_piece[i] == -1) score = 0;
		else{
			int distance = abs(b->cube_position[cube_piece[i]] - target[i]);
			score = 4 - std::max(distance/5, distance%5);
		}
		final_score += i == color ? -score: score;
	}
	return final_score;
}

float Prob::Evaluate_nearest(Board* b, int color, int* cube_piece) {
	float p[6];
	float score = 0.0;
	float final_score = 0.0;
	
	for(int i=0;i<2;i++){
		b->cal_probability(p, 1-i);
		if( cube_piece[i] == -1) score = 0;
		else{
			int index = cube_piece[i] % 6;
			score = p[index];
		}
		final_score += i == color ? -score: score;
	}
	return final_score;
}

// Blue
float Turn::Evaluate_nearest(Board* b, int color, int* cube_piece){
	return -b->color+0.5;
}

// RED
// float Turn::Evaluate_nearest(Board* b, int color, int* cube_piece){
// 	return b->color-0.5;
// }

// float Turn::Evaluate_nearest(Board* b, int color, int* cube_piece){
// 	return b->color == RED? b->color-0.5: -b->color+0.5;
// }

// float Turn::Evaluate_nearest(Board* b, int color, int* cube_piece){
// 	return 1-color;
// }