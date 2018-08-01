#ifndef search_hpp
#define search_hpp
#include <climits>
#include "solutions.hpp"

class board;

struct search_result
{
    search_result(bool blue) : move(0), score(blue ? INT_MIN : INT_MAX) {}
    search_result(uint8_t _move, int _score): move(_move), score(_score) {}
    uint8_t move;
    int score;
};

class move_search
{
public:
    move_search();
    search_result move(bool blue, const board& prev, const scoring_strategy& scores, int depth=1);
private:
    search_result no_brainer_from(bool blue, const board& prev, const scoring_strategy& scores);
    uint8_t search_order[8][8];
};

#endif /* search_hpp */
