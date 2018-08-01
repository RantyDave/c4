#include <iostream>
#include <cstdlib>
#include <ctime>
#include "search.hpp"
#include "board.hpp"
#include "solutions.hpp"

move_search::move_search()
{
    srand(static_cast<unsigned int>(time(NULL)));
    
    //initialise the 8 different search orders
    for (int iter=0; iter<8; iter++) {
        bool picked[] {false, false, false, false, false, false, false, false};
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

search_result move_search::move(bool blue, const board& prev, const scoring_strategy& scores, int depth)
{
    // a zero depth move is a no-brainer
    if (depth==0) {
        return no_brainer_from(blue, prev, scores);
    }
    
    // otherwise a layer of search depth
    int ideal_score=(blue ? INT_MAX : INT_MIN);
    search_result best(blue);
    uint8_t* order=search_order[rand() % 8];
    for (uint8_t iter=0; iter<7; iter++) {
        uint8_t considered_move=*(order+iter);
        if (prev.full(considered_move)) continue;
        
        // make the move we are contemplating
        board working(prev);
//        std::cout << "DEPTH="<< depth << " BLUE=" << blue << "  contemplating-move=" << static_cast<int>(this_move) << std::endl;
        working.move(considered_move, blue ? board::colour_blue : board::colour_red);
        
        // terminate the search if we win
        if (solutions::win(working)==ideal_score) {
//            std::cout << "WIN!" << std::endl;
            return search_result(considered_move, ideal_score);
        }
        
        // make a likely move from the opposition
        search_result opposition { no_brainer_from(!blue, working, scores) };
        working.move(opposition.move, blue ? board::colour_red : board::colour_blue); // colour selection is deliberately the wrong way round
        
        // contemplate the possible 'move after's
        search_result best_leaf=move(blue, working, scores, depth-1);
//        std::cout << "===> leaf-score=" << best_leaf.score << std::endl << std::endl;
        if ((blue && (best_leaf.score>best.score)) || (!blue && (best_leaf.score<best.score))) {
            best.move=considered_move;
            best.score=best_leaf.score;
        }
    }
//    std::cout << "ACTUAL MOVE=" << static_cast<int>(best.move) << std::endl;
    return best;
}

search_result move_search::no_brainer_from(bool blue, const board& prev, const scoring_strategy& scores)
{
    //a single depth move, used to find a likely opponent move
    search_result best(blue);
    uint8_t* order=search_order[rand() % 8];
    for (uint8_t iter=0; iter<7; iter++) {
        uint8_t considered_move=*(order+iter);
        if (prev.full(considered_move)) continue;
        
        board working(prev);
//        std::cout << "blue=" << blue << " contemplating-no-brainer=" << static_cast<int>(move) << std::endl;
        working.move(considered_move, blue ? board::colour_blue : board::colour_red);
        int this_score { solutions::score(working, scores) };
        if ((blue && (this_score>best.score)) || (!blue && (this_score<best.score))) {
            best.score=this_score;
            best.move=considered_move;
        }
//        std::cout << std::endl;
    }
    
    return best;
}



