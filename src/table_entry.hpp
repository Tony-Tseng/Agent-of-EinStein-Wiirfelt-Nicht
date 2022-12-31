#pragma once
#include "board.hpp"

#define LOWER_BOUND 0
#define EXACT_VALUE 1
#define UPPER_BOUND 2


class Table_Entry{
    public:
    bool occupied = false;
    int cube_position[2*PIECE_NUM];
    int color;
    int dice;
    int depth;
    int value_type;
    float best_value;
    int best_move_piece;
    int best_move_start;
    int best_move_end;
    
    Table_Entry(){
    }
    ~Table_Entry(){
    }

    void set_table(Board* b, int depth, float value, int value_type, int* move_list, int move_index);
};