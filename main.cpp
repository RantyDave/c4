#include <iostream>
#include <cstdint>
#include <algorithm>
#include "model/board.hpp"
#include "model/solutions.hpp"
#include "model/search.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    board brd;
    scoring_strategy scores {1, 2, 5};
    
    bool blue=false;
    for (int n=0; n<50; n++) {
        blue=!blue;
        search_result res { move_search::move(blue, brd, scores, blue ? 1 : 3) };
        brd.move(res.move, blue ? board::colour_blue : board::colour_red);
        brd.dump();
        
        int score=solutions::score(brd, scores);
        cout << "score=" << score << endl << endl;
        if ((score==INT_MAX) or (score==INT_MIN)) break;
    }

    return 0;
}



/*
 // single move
 board brd("3");
 brd.dump();
 cout << "rate=" << solutions::score(brd, singles, doubles, trebles) << endl << endl;
 
 // two moves (should clear first)
 board brd2("12");
 brd2.dump();
 cout << "rate=" << solutions::score(brd2, singles, doubles, trebles) << endl << endl;
 
 // blue ahead
 board brd3("162");
 brd3.dump();
 cout << "rate=" << solutions::score(brd3, singles, doubles, trebles) << endl << endl;
 brd3.move(5, board::colour_red);
 brd3.dump();
 cout << "rate=" << solutions::score(brd3, singles, doubles, trebles) << endl << endl;
 
 // blue wins (horizontal)
 board brd4("1626364");
 brd4.dump();
 cout << "rate=" << solutions::score(brd4, singles, doubles, trebles) << endl << endl;
 
 // red wins (vertical)
 board brd5("60605050");
 brd5.dump();
 cout << "rate=" << solutions::score(brd5, singles, doubles, trebles) << endl << endl;
 
 // blue wins (forward slash)
 board brd6("01152223333");
 brd6.dump();
 cout << "rate=" << solutions::score(brd6, singles, doubles, trebles) << endl << endl;
 
 // red wins (back slash)
 board brd7("065504443333");
 brd7.dump();
 cout << "rate=" << solutions::score(brd7, singles, doubles, trebles) << endl << endl;
 */
