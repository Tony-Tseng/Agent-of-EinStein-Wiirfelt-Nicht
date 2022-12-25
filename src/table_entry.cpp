#include "table_entry.hpp"

void Table_Entry::set_table(Board* b, int depth, float value, int value_type, int* move_list, int move_index){
    occupied = true;
    this->value_type = value_type;

    for(int i=0;i<2*PIECE_NUM;i++){
        cube_position[i] = b->cube_position[i];
    }
    color = b->color;
    this->depth = depth;
    best_value = value;
    best_move_piece = move_list[move_index*3];
    best_move_start = move_list[move_index*3+1];
    best_move_end = move_list[move_index*3+2];
}