#ifndef board_hpp
#define board_hpp
#include <cstdint>
#include <algorithm>

struct board
{
    board(const char* turns="");
    
    uint64_t filled { 0 };
    uint64_t possible { 0 };  // bitmask 0=not on this move 1=is an option for this move
    uint64_t colour { 0 };  // bitmask 0=blue 1=red
    inline static uint64_t bitfor(uint8_t x, uint8_t y) { return static_cast<uint64_t>(1) << (x+y*7); }
    
    uint8_t heights[8] { 0, 0, 0, 0, 0, 0, 0, 0 }; // last one is max height
    static const uint64_t colour_blue { 0 };
    static const uint64_t colour_red { 0xffffffffffffffff };
    inline void move(uint8_t x, uint64_t col) {
        uint8_t y=heights[x]++;
        uint64_t bit_x_y=bitfor(x, y);
        filled |= bit_x_y;          // set the filled bit
        possible ^= bit_x_y;        // unset the possibility bit
        possible ^= bit_x_y<<7;     // set the new possibility bit
        colour |= (col & bit_x_y);  // set the colour bit
        heights[7] = std::max(heights[x], heights[7]);
    };
    inline uint8_t max_height() const {
        return heights[7];
    }
    inline bool full(uint8_t x) const {
        return heights[x]==6;
    }
    
    void dump();
};

#endif /* board_hpp */
