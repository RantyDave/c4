#include <iostream>
#include "solutions.hpp"
#include "board.hpp"


using namespace std;

int solutions::score(const board& board, const scoring_strategy& scores)
{
    // first off check for completes
    int total=win(board);
    if (total!=0) return total;
    
    // add together scores for possibilities
    total+=score(board, ".XXX", scores.trebles);
    total+=score(board, "X.XX", scores.trebles);
    total+=score(board, "XX.X", scores.trebles);
    total+=score(board, "XXX.", scores.trebles);
    
    total+=score(board, "XX..", scores.doubles);
    total+=score(board, ".XX.", scores.doubles);
    total+=score(board, "..XX", scores.doubles);
    total+=score(board, "X..X", scores.doubles);

    total+=score(board, "X...", scores.singles);
    total+=score(board, ".X..", scores.singles);
    total+=score(board, "..X.", scores.singles);
    total+=score(board, "...X", scores.singles);
    
//    if (total!=0) cout << "total=" << total << endl;
    return total;
}

// to score, a series of four locations in a row must
// * Be filled where there is an 'X' in the pattern
// * Have all the colours in the filled locations be the same
// * Be available where there is a '.' in the pattern
int solutions::score(const board& board, const char* pattern, int value)
{
    uint64_t mask, mask2;
    uint64_t available, available2;
    int score=0;

    // a mask of each of the locations we will be checking to see if they are filled
    mask=mask_interpret(pattern, horizontal_coordinates);
    // a mask of each of the locations we will be checking to see if they are available
    available=available_interpret(pattern, horizontal_coordinates);
    for (uint8_t y=0; y<board.max_height(); y++) {
        for (uint8_t x=0; x<4; x++) {
            // (geometrically) translate the mask over to these x and y offsets
            uint64_t mask_translated=mask_translate(mask, x, y);
            uint64_t available_translated=mask_translate(available, x, y);
            // if we lay the mask over board.filled and it is == then each of these positions are filled
            // if we lay the 'available' mask over board.available and it is == then each of those positions are available
            if (((mask_translated & board.filled)==mask_translated) && ((available_translated & board.possible)==available_translated)){
                // if we lay the mask over board.colour and it is == 0 then all the masked positions are blue
                if ((mask_translated & board.colour)==0) {
//                    cout << "blue horizontal " << pattern << " x=" << static_cast<int>(x) << " y=" << static_cast<int>(y) << " score=" << value << endl;
                    score+=value;
                }
                // if we lay the mask over board.colour and it is == then all the masked positions are red
                if ((mask_translated & board.colour)==mask_translated) {
//                    cout << "red horizontal " << pattern << " x=" << static_cast<int>(x) << " y=" << static_cast<int>(y) << " score=" << -value << endl;
                    score-=value;
                }
            }
        }
    }
    
    mask=mask_interpret(pattern, vertical_coordinates);
    available=available_interpret(pattern, vertical_coordinates);
    for (uint8_t y=0; y<board.max_height()-3; y++) {
        for (uint8_t x=0; x<7; x++) {
            uint64_t mask_translated=mask_translate(mask, x, y);
            uint64_t available_translated=mask_translate(available, x, y);
            if (((mask_translated & board.filled)==mask_translated) && ((available_translated & board.possible)==available_translated)){
                if ((mask_translated & board.colour)==0) {
//                    cout << "blue vertical " << pattern << " x=" << static_cast<int>(x) << " y=" << static_cast<int>(y) << " score=" << value << endl;
                    score+=value;
                }
                if ((mask_translated & board.colour)==mask_translated) {
//                    cout << "red vertical " << pattern << " x=" << static_cast<int>(x) << " y=" << static_cast<int>(y) << " score=" << value << endl;
                    score-=value;
                }
            }
        }
    }
    
    mask=mask_interpret(pattern, fslash_coordinates);
    mask2=mask_interpret(pattern, bslash_coordinates);
    available=available_interpret(pattern, fslash_coordinates);
    available2=available_interpret(pattern, bslash_coordinates);
    for (uint8_t y=0; y<board.max_height()-3; y++) {
        for (uint8_t x=0; x<4; x++) {
            uint64_t mask_translated=mask_translate(mask, x, y);
            uint64_t available_translated=mask_translate(available, x, y);
            if (((mask_translated & board.filled)==mask_translated) && ((available_translated & board.possible)==available_translated)){
                if ((mask_translated & board.colour)==0) {
//                    cout << "blue slash " << pattern << " x=" << static_cast<int>(x) << " y=" << static_cast<int>(y) << " score=" << value << endl;
                    score+=value;
                }
                if ((mask_translated & board.colour)==mask_translated) {
//                    cout << "red slash " << pattern << " x=" << static_cast<int>(x) << " y=" << static_cast<int>(y) << " score=" << value << endl;
                    score-=value;
                }
            }
            uint64_t mask_translated2=mask_translate(mask2, x, y);
            uint64_t available_translated2=mask_translate(available2, x, y);
            if (((mask_translated2 & board.filled)==mask_translated2) && ((available_translated2 & board.possible)==available_translated2)){
                if ((mask_translated2 & board.colour)==0) {
//                    cout << "blue backslash " << pattern << " x=" << static_cast<int>(x) << " y=" << static_cast<int>(y) << " score=" << value << endl;
                    score+=value;
                }
                if ((mask_translated2 & board.colour)==mask_translated2) {
//                    cout << "red backslash " << pattern << " x=" << static_cast<int>(x) << " y=" << static_cast<int>(y) << " score=" << value << endl;
                    score-=value;
                }
            }
        }
    }
    
    return score;
}

uint64_t solutions::mask_interpret(const char* in, const coordset& crds)
{
    return  (in[0]=='X' ? board::bitfor(crds[0][0], crds[0][1]) : 0) |
            (in[1]=='X' ? board::bitfor(crds[1][0], crds[1][1]) : 0) |
            (in[2]=='X' ? board::bitfor(crds[2][0], crds[2][1]) : 0) |
            (in[3]=='X' ? board::bitfor(crds[3][0], crds[3][1]) : 0);
}

uint64_t solutions::available_interpret(const char* in, const coordset& crds)
{
    return  (in[0]=='.' ? board::bitfor(crds[0][0], crds[0][1]) : 0) |
            (in[1]=='.' ? board::bitfor(crds[1][0], crds[1][1]) : 0) |
            (in[2]=='.' ? board::bitfor(crds[2][0], crds[2][1]) : 0) |
            (in[3]=='.' ? board::bitfor(crds[3][0], crds[3][1]) : 0);
}
