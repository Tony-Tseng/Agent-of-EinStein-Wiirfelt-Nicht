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
	int depth_limit = 7;
	float hard_time_limit = 100.0;
	float threshold = 2;
	const int num_strategy = 2;
	float weight[2] = {1, 0.17};
	Strategy* strategy[2] = {new CubeStep(), new Prob()};
	// const int num_strategy = 3;
	// float weight[3] = {1, 0.17, 0.3};
	// Strategy* strategy[3] = {new CubeStep(), new Prob(), new Piece()};
	// const int num_strategy = 3;
	// float weight[3] = {1, 0.17, 0.5};
	// Strategy* strategy[3] = {new CubeStep(), new Prob(), new Turn()};
	// const int num_strategy = 1;
	// float weight[1] = {1};
	// Strategy* strategy[1] = {new Manhattan()};

	float MINVALUE = 0.0;
	float MAXVALUE = 0.0;
	

	NegaScout(Board *b){
		set_root(b);
	};
	NegaScout(){
	};
	~NegaScout(){
	};
	double timer(bool reset);

	void set_root(Board* b);
	void set_time_limit(double time_limit);
	void Generate_random_move(char* move);
	void Generate_move(char* move);

	std::pair<int, float> First_F(float alpha, float beta, int depth);
	
	float Star1_F(Board* b, float alpha, float beta, int depth);
	float Star1_G(Board* b, float alpha, float beta, int depth);
	float Search_F(Board* b, int next_dice, float alpha, float beta, int depth);
	float Search_G(Board* b, int next_dice, float alpha, float beta, int depth);

	float evaluate(Board* b);
	void Get_nearest(Board* b, int* cube_piece);
	float truncate(float value, int num);

private:
	Board* root = new Board();
	Table* transposition_table = new Table();
};