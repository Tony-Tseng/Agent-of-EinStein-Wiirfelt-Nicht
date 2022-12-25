#pragma once
#include "board.hpp"
#include "table_entry.hpp"
#include "iostream"

#define TABLE_N 18
#define TABLE_M 7

class Table{
    public:
    Table_Entry* table;
    int hash_table[12][25];
    int hash_color[2];
    int max_entry;
    int max_hash;
    int SEED = 1111;

    Table(){
        max_entry = pow(2.0, TABLE_N);
        max_hash = pow(2.0, TABLE_M+TABLE_N);
        table = new Table_Entry[max_entry];
        Generate_hash_table();
    }
    ~Table(){
        delete[] table;
    }

    void Generate_hash_table();
    void Set_entry(Board* b, int _depth, float value, int value_type, int* move_list, int move_index);
    int Calculate_hash(Board* b);
    int Calculate_hash_by_move(Board* b, int hash_value, int piece, int start_pos, int end_pos);
    int Get_board_index(Board* b);
    bool Check_same_state(Board* b, int index);
};