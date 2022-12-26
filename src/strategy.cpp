#include "strategy.hpp"


float DefaultEvaluate::Evaluate(Board* b, int color) {
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
		final_score += i == color ? -score: score;
	}
	return final_score;
}