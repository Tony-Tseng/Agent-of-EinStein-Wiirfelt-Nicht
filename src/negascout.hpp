#pragma once

#include <time.h>
#include <math.h>
#include "board.hpp"
#include "table.hpp"
#include "strategy.hpp"

class NegaScout
{
public:
	struct timespec tick, tock;
	double time_limit = 10.0;
	int depth_limit = 5;
	float threshold = 2;
	const int num_strategy = 2;
	float weight[3] = {0.81488095, 0.18038665, 0.07416915, };
	float bias = 0.02727044;
	Strategy* strategy[3] = {new CubeStep(), new Prob(), new Manhattan()};
	float MINVALUE = 0.0;
	float MAXVALUE = 0.0;
	

	NegaScout(Board *b){
		*root = *b;
		int hash_value = transposition_table->Calculate_hash(root);
		root->hash_value = hash_value;

		std::pair<float, float> bound;
		for(int i=0;i<num_strategy;i++){
			bound = strategy[i]->GetBound();
			MINVALUE += bound.first * weight[i];
			MAXVALUE += bound.second * weight[i];
		}
		MINVALUE += bias;
		MAXVALUE += bias;
	};
	~NegaScout(){
	};
	double timer(bool reset);

	void Generate_random_move(char* move);
	void Generate_move(char* move);

	std::pair<int, float> First_F(float alpha, float beta, int depth);
	
	float Star1_F(Board* b, float alpha, float beta, int depth);
	float Star1_G(Board* b, float alpha, float beta, int depth);
	float Search_F(Board* b, float alpha, float beta, int depth);
	float Search_G(Board* b, float alpha, float beta, int depth);

	// float evaluate(Board* b, int color);
	float evaluate(Board* b);
	void Get_nearest(Board* b, int* cube_piece);
	float truncate(float value, int num);

private:
	Board* root = new Board();
	Table* transposition_table = new Table();
};