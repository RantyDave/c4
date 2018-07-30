#include <iostream>
#include <cstdint>
#include <algorithm>
#include "model/board.hpp"
#include "model/solutions.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    board brd;
    int singles=1;
    int doubles=100;
    int trebles=10000;
    
    // single move
    brd.init("3");
    brd.dump();
    cout << "rate=" << solutions::score(brd, singles, doubles, trebles) << endl << endl;
    
    // two moves (should clear first)
    brd.init("12");
    brd.dump();
    cout << "rate=" << solutions::score(brd, singles, doubles, trebles) << endl << endl;
    
    // blue ahead
    brd.init("162");
    brd.dump();
    cout << "rate=" << solutions::score(brd, singles, doubles, trebles) << endl << endl;
    brd.move(5, board::colour_red);
    brd.dump();
    cout << "rate=" << solutions::score(brd, singles, doubles, trebles) << endl << endl;
    
    // blue wins (horizontal)
    brd.init("1626364");
    brd.dump();
    cout << "rate=" << solutions::score(brd, singles, doubles, trebles) << endl << endl;

    // red wins (vertical)
    brd.init("60605050");
    brd.dump();
    cout << "rate=" << solutions::score(brd, singles, doubles, trebles) << endl << endl;

    // blue wins (forward slash)
    brd.init("01152223333");
    brd.dump();
    cout << "rate=" << solutions::score(brd, singles, doubles, trebles) << endl << endl;

    // red wins (back slash)
    brd.init("065504443333");
    brd.dump();
    cout << "rate=" << solutions::score(brd, singles, doubles, trebles) << endl << endl;

    return 0;
}
