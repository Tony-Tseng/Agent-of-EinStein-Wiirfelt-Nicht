#include "node.hpp"

void Node::set_node(Board* _b, Move _cube_move, int _p_id, int _depth){
    *state = *_b;
    cube_move = _cube_move;
    p_id = _p_id;
    depth = _depth;
}