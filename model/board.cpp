#include "board.hpp"
#include <iostream>

using namespace std;

void board::move(uint8_t x, bool blue)
{
    // do the move
    uint8_t y=heights[x]++;  // increment height of the column
    uint64_t bit_x_y=bitfor(x, y);  // get a single bit that is used as a mask
    filled |= bit_x_y;          // set the filled bit
    if (!blue) colour |= bit_x_y;  // (optionally) set the colour bit
    heights[7] = max(heights[x], heights[7]);
}

// to score, a series of four locations in a row must
// * Be filled
// * Have all the colours in the filled locations be the same
int board::score()
{
    uint64_t mask, mask2;
    
    // a mask of each of the locations we will be checking to see if they are filled
    mask=mask_from_coords(horizontal_coordinates);
    for (uint8_t y=0; y<max_height(); y++) {
        for (uint8_t x=0; x<4; x++) {
            // (geometrically) translate the mask over to these x and y offsets
            uint64_t mask_translated=mask_translate(mask, x, y);
            // if we lay the mask over board.filled and it is == then each of these positions are filled
            // if we lay the 'available' mask over board.available and it is == then each of those positions are available
            if ((mask_translated & filled)==mask_translated) {
                // if we lay the mask over board.colour and it is == 0 then all the masked positions are blue
                if ((mask_translated & colour)==0) return INT_MAX;
                // if we lay the mask over board.colour and it is == then all the masked positions are red
                if ((mask_translated & colour)==mask_translated) return -INT_MAX;
            }
        }
    }
    
    mask=mask_from_coords(vertical_coordinates);
    for (uint8_t y=0; y<max_height()-3; y++) {
        for (uint8_t x=0; x<7; x++) {
            uint64_t mask_translated=mask_translate(mask, x, y);
            if ((mask_translated & filled)==mask_translated) {
                if ((mask_translated & colour)==0) return INT_MAX;
                if ((mask_translated & colour)==mask_translated) return -INT_MAX;
            }
        }
    }
    
    mask=mask_from_coords(fslash_coordinates);
    mask2=mask_from_coords(bslash_coordinates);
    for (uint8_t y=0; y<max_height()-3; y++) {
        for (uint8_t x=0; x<4; x++) {
            uint64_t mask_translated=mask_translate(mask, x, y);
            if ((mask_translated & filled)==mask_translated) {
                if ((mask_translated & colour)==0) return INT_MAX;
                if ((mask_translated & colour)==mask_translated) return -INT_MAX;
            }
            uint64_t mask_translated2=mask_translate(mask2, x, y);
            if ((mask_translated2 & filled)==mask_translated2) {
                if ((mask_translated2 & colour)==0) return INT_MAX;
                if ((mask_translated2 & colour)==mask_translated2) return -INT_MAX;
            }
        }
    }
    return 0;
}

void board::dump(int indent)
{
    for (uint8_t y=5; y<6; y--) {  // rolls round because unsigned
        cout << string(indent, ' ');
        for (uint8_t x=0; x<7; x++) {
            uint64_t bit_x_y=bitfor(x, y);
            if (filled & bit_x_y) {
                cout << ((colour & bit_x_y) ? "R" : "B");
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
    cout << string(indent, ' ') << "0123456" << endl;
}
