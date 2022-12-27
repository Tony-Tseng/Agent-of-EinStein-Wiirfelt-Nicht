#pragma once

#include <time.h>
#include <math.h>
#include "board.hpp"
#include "table.hpp"

#define MAXNODE 1
#define MAXVALUE 8
#define MINVALUE -8

class NegaScout
{
public:
	struct timespec tick, tock;
	double time_limit = 10.0;
	int depth_limit = 5;
	float threshold = 2;
	NegaScout(Board *b){
		*root = *b;
		int hash_value = transposition_table->Calculate_hash(root);
		root->hash_value = hash_value;
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

	float evaluate(Board* b, int color);
	float truncate(float value, int num);

private:
	Board* root = new Board();
	Table* transposition_table = new Table();
};