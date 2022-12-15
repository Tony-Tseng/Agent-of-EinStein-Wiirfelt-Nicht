#pragma once

#include "board.hpp"
#include "node.hpp"

#define MAXNODE 1000000


class NegaScout  
{
public:
	NegaScout(){
		tree_node = new Node[MAXNODE];
	};
	~NegaScout(){
		delete tree_node;
	};

	void add_node(Board* b, Move move, int p_id, int depth);
	
	void Generate_random_move(char* move);
	void Generate_move(char* move);

	float evaluate(Board* b, int color);

	// Move NegaScout_Move(Board* b, float alpha, float beta, int depth, int color);

private:
	Node* tree_node;
	int head_index = 0;
};