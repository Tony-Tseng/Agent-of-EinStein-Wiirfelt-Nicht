#pragma once
#include "board.hpp"

// using PII = std::pair<int, int>;

class Move{
    public:
    int piece;
    int start_point;
    int end_point;

    Move(){}; // default constructor
    Move(int _piece, int _start_point, int _end_point){
        piece = _piece;
        start_point = _start_point;
        end_point = _end_point;
    }
    ~Move(){};
    
};

class Node{
    public:
        Board* state;
        Move cube_move;
        int c_id[18];
        int N_child;
        int p_id;
        int depth;
        float score;

        
        Node(){
            state = new Board();
            memset(c_id, 0, sizeof(c_id));
        }
        ~Node(){
            delete state;
        }

        void set_node(Board* _b, Move _cube_move, int _p_id, int _depth);
};