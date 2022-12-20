#pragma once

#include "board.hpp"
#include "node.hpp"

#define MAXNODE 1000000


class NegaScout  
{
public:
	int search_depth = 3;
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
	float Search(Board* b, float alpha, float beta, int depth);

	float evaluate(Board* b, int color);

private:
	Node* tree_node;
	int head_index = 0;
};