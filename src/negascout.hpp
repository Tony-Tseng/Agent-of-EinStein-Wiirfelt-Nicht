#pragma once

#include "board.hpp"
#include "table.hpp"

#define MAXNODE 1
#define MAXVALUE 8
#define MINVALUE -8

class NegaScout
{
public:
	int search_depth = 6;
	NegaScout(Board *b){
		*root = *b;
		int hash_value = transposition_table->Calculate_hash(root);
		root->hash_value = hash_value;
	};
	~NegaScout(){
	};

	void Generate_random_move(char* move);
	void Generate_move(char* move);
	
	float Star1_F(Board* b, float alpha, float beta, int depth);
	float Star1_G(Board* b, float alpha, float beta, int depth);
	float Search_F(Board* b, float alpha, float beta, int depth);
	float Search_G(Board* b, float alpha, float beta, int depth);

	float evaluate(Board* b, int color);

private:
	Board* root = new Board();
	Table* transposition_table = new Table();
};