#include "table.hpp"

void Table::Set_entry(Board* b, int _depth, float value, int value_type, int* move_list, int move_index){
    int index = Get_board_index(b);
    table[index].set_table(b, _depth, value, value_type, move_list, move_index);
}

int Table::Calculate_hash_by_move(Board* b, int next_dice, int hash_value, int piece, int start_pos, int end_pos){
    if(b->board[end_pos/5][end_pos%5] != 0){
        hash_value ^= hash_table[b->board[end_pos/5][end_pos%5]-1][end_pos];
    }
    hash_value ^= hash_dice[b->dice];
    hash_value ^= hash_dice[next_dice];
    hash_value ^= hash_table[piece-1][start_pos];
    hash_value ^= hash_table[piece-1][end_pos];
    hash_value ^= hash_color[0];
    hash_value ^= hash_color[1];

    return hash_value;
}

int Table::Calculate_hash(Board* b){
    int hash = hash_color[b->color];
    for(int i=0;i<2*PIECE_NUM;i++){
        hash ^= hash_table[i][b->cube_position[i]];
    }
    hash ^= hash_dice[b->dice];
    return hash;
}

int Table::Get_board_index(Board* b){
    return Calculate_hash(b) % max_entry;
}

bool Table::Check_same_state(Board* b, int index){
    Table_Entry entry = table[index];

    if(b->color != entry.color){
        return false;
    }

    if(b->dice != entry.dice){
        return false;
    }

    for(int i=0;i<2*PIECE_NUM;i++){
        if(entry.cube_position[i] != b->cube_position[i]){
            return false;
        }
    }
    return true;
}

void Table::Generate_hash_table(){
    srand(SEED);

    for(int i=0;i<12;i++){
        for(int j=0;j<25;j++){
            hash_table[i][j] = rand() % max_hash;
        }
    }

    for(int i=0;i<6;i++){
        hash_dice[i] = rand() % max_hash;
    }
    hash_color[0] = rand() % max_hash;
    hash_color[1] = rand() % max_hash;
}