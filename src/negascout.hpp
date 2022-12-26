#pragma once

#include <math.h>
#include "board.hpp"
#include "node.hpp"

#define MAXNODE 1000000
#define MAXVALUE 8
#define MINVALUE -8


class NegaScout
{
public:
	int search_depth = 5;
	NegaScout(){
		tree_node = new Node[MAXNODE];
	};
	~NegaScout(){
		delete [] tree_node;
	};

	void add_node(Board* b, Move move, int p_id, int depth);
	
	void Generate_random_move(char* move);
	void Generate_move(char* move);
	
	float Star0_search(Board* b, float alpha, float beta, int depth);
	float Star0_F(Board* b, float alpha, float beta, int depth);
	float Star0_G(Board* b, float alpha, float beta, int depth);
	float Search_F(Board* b, float alpha, float beta, int depth);
	float Search_G(Board* b, float alpha, float beta, int depth);
	
	float Star1_search(Board* b, float alpha, float beta, int depth);
	float Search(Board* b, float alpha, float beta, int depth);

	float evaluate(Board* b, int color);

private:
	Node* tree_node;
	int head_index = 0;
};