#include "board.hpp"
#include <iostream>

using namespace std;

// one off init, don't want to put it in the c'tor
// because that would break bitwise copying
void board::init(const char* turn="")
{
    // clear
    filled=0;
    colour=0;
    possible=0;
    
    // all the bottom locations are possible
    for (uint8_t x=0; x<7; x++) {
        possible |= bitfor(x, 0);
        heights[x]=0;
    }
    heights[7]=0; // max height is zero
    
    // initial turns (for testing)
    int red=false;
    while (*turn!=0) {
        int column=*turn-'0';
        move(column, red ? colour_red : colour_blue);
        red=!red;
        ++turn;
    }
}

void board::dump()
{
    for (uint8_t y=5; y<6; y--) {  // rolls round because unsigned
        for (uint8_t x=0; x<7; x++) {
            uint64_t bit_x_y=bitfor(x, y);
            if (possible & bit_x_y) {
                cout << "?";
            } else if (filled & bit_x_y) {
                cout << ((colour & bit_x_y) ? "R" : "B");
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
    for (uint8_t x=0; x<7; x++) {
        cout << static_cast<int>(heights[x]);
    }
    cout << endl;
}
