#include <iostream>
#include <cstdint>
#include "model/board.hpp"
#include "model/search.hpp"

using namespace std;

int main(int argc, const char * argv[])
{
    board brd;
    search_tree srch(brd);
    search_orders ordering;
    uint32_t root_node=0;
    
    bool blue=true;
    for (unsigned int n=0; n<42; n++) {  // max of 7*6=42 moves
        std::cout << "examining options for " << (blue ? "blue" : "red") << std::endl;
        score_options options;  // options need to be reset to zero every time, don't recycle this object
        srch.options(&options, root_node, blue);
        uint8_t move=options.best_move(blue, ordering);
        std::cout << "taking move=" << static_cast<int>(move) << std::endl;
        root_node=srch.prune_from(root_node, move);
        srch.node_at(root_node)->brd.dump();
        if ((srch.node_at(root_node)->board_score==INT_MAX) or (srch.node_at(root_node)->board_score==-INT_MAX)) {
            std::cout << (srch.node_at(root_node)->board_score==INT_MAX ? "blue wins" : "red wins") << std::endl;
            break;
        }
        std::cout << std::endl;
        blue=!blue;
    }

    return 0;
}

