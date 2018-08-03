#include <iostream>
#include <cstdlib>
#include <ctime>
#include "search.hpp"
#include "board.hpp"

search_tree::search_tree(const board& brd)
{
    //allocate enough space to have a million boards in the prediction cache at any one time
    nodes=static_cast<search_node*>(malloc(sizeof(search_node)*1048576));
    
    //mark all the slots as being available
    memset(static_cast<void*>(node_slot_available), 1, 1048576);
    
    //create the root node
    new(&nodes[0]) search_node(brd);
    node_slot_available[0]=0;
}

search_tree::~search_tree()
{
    free(nodes);
}

void search_tree::options(score_options* in, uint32_t root_node_slot, bool blue, uint32_t depth)
{
    recurse_options(in, root_node_slot, blue, depth, -1, 0);
}

// root_eval_move is -1 if called from the 'actual' move
void search_tree::recurse_options(score_options* in, uint32_t node_slot, bool blue, uint32_t depth, int root_eval_move, uint32_t layer)
{
    search_node* node=&nodes[node_slot];
    
    // find or spawn a new node for each possible move
    uint32_t child_node_slot=0;
    for (int eval_mve=0; eval_mve<7; eval_mve++) {
        // ensure we 'credit' results to the move at the base of the tree
        int this_root_eval_move=(root_eval_move==-1) ? eval_mve : root_eval_move;
        
        // bin out if the column is full so we can't make this move anyway
        if (node->brd.full(eval_mve)) {
            // mark as invalid if it's the root move that can't be made
            if (root_eval_move==-1) in->valid[this_root_eval_move]=false;
            continue;
        }
        
        // ensure the child node has been created
        child_node_slot=node->fwd_id[eval_mve];
        if (child_node_slot==-1) {  // haven't created the child node yet
            child_node_slot=find_empty_slot();
//            std::cout << "allocating: " << child_node_slot << std::endl;
            node->fwd_id[eval_mve]=child_node_slot;
            search_node* new_node=new(&nodes[child_node_slot]) search_node(node->brd);
            node_slot_available[child_node_slot]=0;
            new_node->brd.move(eval_mve, blue);
            new_node->board_score=new_node->brd.score();
        }
        
        // find the score for this move
        int child_board_score;
        search_node* child_node=&nodes[child_node_slot];
        child_board_score=child_node->board_score;
        int this_score=child_board_score >> (layer*3);
        if (this_score!=0) {
//            std::cout << std::string(layer*2, ' ') << "child_node=" << child_node_slot << " layer=" << layer << " root_eval_move=" << root_eval_move << " child_board_score=" << child_board_score << " score=" << this_score << std::endl;
//            child_node->brd.dump(layer*2);
            in->scores[this_root_eval_move]+=this_score;
            continue;
        }
        
        // if we've not hit a result or the depth limit, try again with the next layer down
        if (depth) {
            recurse_options(in, child_node_slot, !blue, depth-1, this_root_eval_move, layer+1);
        }
    }
}

uint32_t search_tree::prune_from(uint32_t node_slot, uint8_t except)
{
    search_node* node=&nodes[node_slot];
    uint32_t new_root_node=0; //only actually gets set if there's a valid except
    
    for (uint8_t mve=0; mve<7; mve++) {
        // not allocated?
        if (node->fwd_id[mve]==-1) continue;
        
        // child node
        uint32_t child_node_slot=node->fwd_id[mve];
        if (mve==except) {
            new_root_node=child_node_slot;  // the new root slot id
//            std::cout << "keeping: " << child_node_slot << std::endl;
            continue;  // don't nuke it
        }
//        std::cout << "recursing: " << node_slot << "-->" << child_node_slot << std::endl;
        prune_from(child_node_slot);  // delete child node
    }
    
    //free this allocation
    node_slot_available[node_slot]=1;
//    std::cout << "freeing: " << node_slot << std::endl;
//    if (new_root_node!=0) std::cout << "new root node: " << new_root_node << std::endl;
    return new_root_node;
}

uint32_t search_tree::find_empty_slot()
{
//    uint32_t iters=0;
    uint32_t loop_round_at=last_slot_allocated;
    while (++last_slot_allocated) {
        if (last_slot_allocated==1048576) last_slot_allocated=0;  // loop round
        if (node_slot_available[last_slot_allocated]) {
//            if (iters!=0) std::cout << "find_empty_slot iterations=" << iters << std::endl;
            return last_slot_allocated;
        }
        if (last_slot_allocated==loop_round_at) {
            std::cerr << "Ran out of node slots" << std::endl;
            exit(1);
        }
//        ++iters;
    }
    std::cerr << "Should not be seeing this" << std::endl;
    exit(1);
}

search_orders::search_orders()
{
    srand(static_cast<unsigned int>(time(NULL)));
    
    //use 8 different search orders so we get a randomised tie breaker
    for (int iter=0; iter<8; iter++) {
        bool picked[] {false, false, false, false, false, false, false};
        for (int pick=0; pick<7; pick++) {
            //keep trying until we pick an unused one
            int this_one;
            do {
                this_one=rand() % 7;
            } while (picked[this_one]==true);
            picked[this_one]=true;
            search_order[iter][pick]=this_one;
        }
    }
}

uint8_t score_options::best_move(bool blue, const search_orders& ordering)
{
    // pick one of the 8 search orders
    int pattern=rand() % 8;
    const uint8_t* search_id=ordering.search_order[pattern];
    
    // pick the best score
    int best_score=blue ? -INT_MAX : INT_MAX;
    int best_move=-1;
    for (uint32_t idx=0; idx<7; idx++) {
        uint8_t score_idx=search_id[idx];
        if (!valid[score_idx]) continue;
        if ((blue and (scores[score_idx]>best_score)) or (!blue and (scores[score_idx]<best_score))) {
            best_score=scores[score_idx];
            best_move=score_idx;
        }
    }
    return best_move;
}

void score_options::dump()
{
    for (int n=0; n<7; n++) std::cout << n << "->" << scores[n] << (valid[n] ? " " : "(invalid) ");
    std::cout << std::endl;
}

void search_tree::dump_from(uint32_t node_slot, uint32_t layer)
{
    search_node* node=&nodes[node_slot];
    std::cout << std::endl << std::string(layer*2, '-') << "slot=" << node_slot << std::endl;
    node->brd.dump(layer*2);
    std::cout << std::string(layer*2, ' ') << "score=" << node->board_score << std::endl;
    for (int mve=0; mve<7; mve++) {
        if (node->fwd_id[mve]==-1) {
            continue;
        }
        
        dump_from(node->fwd_id[mve], layer+1);
    }
}

