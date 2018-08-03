#ifndef search_hpp
#define search_hpp
#include "board.hpp"
#include <climits>

struct search_node
{
    search_node(const board& _brd) : brd(_brd), board_score(0) {};
    board brd;
    int board_score;
    int fwd_id[7] { -1, -1, -1, -1, -1, -1, -1 }; // index of the nodes for each forward move
};

struct search_orders
{
    search_orders();
    uint8_t search_order[8][8];
};

struct score_options
{
    int scores[8] { 0, 0, 0, 0, 0, 0, 0, 0 };  //only 7 are used
    bool valid[8] { true, true, true, true, true, true, true, true};  // whether or not this move is valid
    uint8_t best_move(bool blue, const search_orders& ordering);
    void dump();
};

class search_tree
{
public:
    search_tree(const board& brd);
    ~search_tree();
    void options(score_options* in, uint32_t root_node_slot, bool blue, uint32_t depth=6);
    uint32_t prune_from(uint32_t node_slot, uint8_t except=99); // except describes which move we took, returns new root node_slot if except was specified
    inline search_node* node_at(uint32_t node_slot) { return &nodes[node_slot]; }
    void dump_from(uint32_t node_slot, uint32_t layer=0);
    
private:
    void recurse_options(score_options* in, uint32_t root_node_slot, bool blue, uint32_t depth, int root_eval_move, uint32_t layer);
    
    // nodes are allocated from a fixed block of 1048576
    search_node* nodes;
    uint8_t node_slot_available[1048576];  // individual byte flags for which slots have
    uint32_t last_slot_allocated { 0 };  // as in, most recent
    uint32_t find_empty_slot();
};

#endif /* search_hpp */
