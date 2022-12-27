#include "strategy.hpp"


float Manhattan::Evaluate(Board* b, int color) {
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

float CubeStep::Evaluate(Board* b, int color) {
	int target[2] = {0, 24};
	float score = 0.0;
	float final_score = 0.0;
	for(int i=0;i<2;i++){
		score = 0.0;
		for(int j=0;j<6;j++){
			if(b->cube_position[i*6+j] == -1) continue;
			int distance = abs(b->cube_position[i*6+j] - target[i]);
			score = std::max( score, (float) (4.0 - std::max(distance/5, distance%5) ) );
		}
		final_score += i == color ? -score: score;
	}
	return final_score;
}

float Manhattan_Prob::Evaluate(Board* b, int color) {
	float p[6];
	int target[2] = {0, 24};
	float score = 0.0;
	float final_score = 0.0;
	for(int i=0;i<2;i++){
		score = 0.0;
		b->cal_probability(p, 1-i);
		for(int j=0;j<6;j++){
			if(b->cube_position[i*6+j] == -1) continue;
			int distance = abs(b->cube_position[i*6+j] - target[i]);
			score = std::max( score, (float) (8.0 - (distance/5 + distance%5))+ p[j] );
		}
		final_score += i == color ? -score: score;
	}
	return final_score;
}

float Threaten::Evaluate(Board* b, int color) {
	float score = 0.0;
	float final_score = 0.0;
	for(int i=0;i<2;i++){
		score = 0.0;
		for(int j=0;j<6;j++){
			if(b->cube_position[i*6+j] == -1) continue;
			score = std::max( score, 6 - GetEnemy(b, i*6+j));
		}
		final_score += i == color ? -score: score;
	}
	return final_score;
}

float Threaten::GetEnemy(Board* b, int cube_num) {
	int i = cube_num/6;

	float score = 0.0;
	int pos, enemy_pos;

	if(i==0){
		pos = b->cube_position[cube_num];
		for(int j=6;j<12;j++){
			if(enemy_pos == -1) continue;
			enemy_pos = b->cube_position[j];
			if(enemy_pos/5 <= pos/5 && enemy_pos%5<=pos%5) score += 1;
		}
	}
	else if(i==1){
		pos = b->cube_position[cube_num];
		for(int j=0;j<6;j++){
			if(enemy_pos == -1) continue;
			enemy_pos = b->cube_position[j];
			if(enemy_pos/5 >= pos/5 && enemy_pos%5 >= pos%5) score += 1;
		}
	}

	return score;
}