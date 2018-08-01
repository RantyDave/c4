#include <iostream>
#include <cstdlib>
#include "search.hpp"
#include "board.hpp"
#include "solutions.hpp"

search_result move_search::move(bool blue, const board& prev, const scoring_strategy& scores, int depth)
{
    // a zero depth move is a no-brainer
    if (depth==0) {
        return no_brainer_from(blue, prev, scores);
    }
    
    // otherwise a layer of search depth
    int ideal_score=(blue ? INT_MAX : INT_MIN);
    search_result best(blue);
    for (uint8_t this_move=0; this_move<7; this_move++) {
        if (prev.full(this_move)) continue;
        
        // make the move we are contemplating
        board working(prev);
//        std::cout << "DEPTH="<< depth << " BLUE=" << blue << "  contemplating-move=" << static_cast<int>(this_move) << std::endl;
        working.move(this_move, blue ? board::colour_blue : board::colour_red);
        
        // terminate the search if we win
        if (solutions::win(working)==ideal_score) {
//            std::cout << "WIN!" << std::endl;
            return search_result(this_move, ideal_score);
        }
        
        // make a likely move from the opposition
        search_result opposition { no_brainer_from(!blue, working, scores) };
        working.move(opposition.move, blue ? board::colour_red : board::colour_blue); // colour selection is deliberately the wrong way round
        
        // contemplate the possible 'move after's
        search_result best_leaf=move(blue, working, scores, depth-1);
//        std::cout << "===> leaf-score=" << best_leaf.score << std::endl << std::endl;
        if ((blue && (best_leaf.score>best.score)) || (!blue && (best_leaf.score<best.score))) {
            best.move=this_move;
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
    for (uint8_t move=0; move<7; move++) {
        if (prev.full(move)) continue;
        
        board working(prev);
//        std::cout << "blue=" << blue << " contemplating-no-brainer=" << static_cast<int>(move) << std::endl;
        working.move(move, blue ? board::colour_blue : board::colour_red);
        int this_score { solutions::score(working, scores) };
        if ((blue && (this_score>best.score)) || (!blue && (this_score<best.score))) {
            best.score=this_score;
            best.move=move;
        }
//        std::cout << std::endl;
    }
    
    return best;
}



