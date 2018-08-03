#include <iostream>
#include <cstdint>
#include <algorithm>
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
    for (unsigned int n=0; n<72; n++) {
        std::cout << "examining options blue=" << blue << std::endl;
        score_options options;
        srch.options(&options, root_node, blue);
        uint8_t move=options.best_move(blue, ordering);
        std::cout << "taking move=" << static_cast<int>(move) << std::endl;
        root_node=srch.prune_from(root_node, move);
        srch.node_at(root_node)->brd.dump();
        if ((srch.node_at(root_node)->board_score==INT_MAX) or (srch.node_at(root_node)->board_score==-INT_MAX)) {
            std::cout << "WIN! " << srch.node_at(root_node)->board_score << std::endl;
            break;
        }
        std::cout << std::endl;
        blue=!blue;
    }

    return 0;
}

