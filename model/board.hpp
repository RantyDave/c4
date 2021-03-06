#ifndef board_hpp
#define board_hpp
#include <cstdint>
#include <algorithm>

class board
{
public:
    void move(uint8_t x, bool blue);
    int score();  // blue is trying for +ve, red is trying for -ve

    inline bool full(uint8_t x) const {return heights[x]==6;}
    inline uint8_t max_height() const {return heights[7];}
    void dump(int indent=0);
    
private:
    uint64_t filled { 0 };  // bitmask 0=empty 1=filled
    uint64_t colour { 0 };  // bitmask 0=blue 1=red
    uint8_t heights[8] { 0, 0, 0, 0, 0, 0, 0, 0 }; // last one is max height
    
    typedef uint8_t coordset[4][2];  // a set of 4 xy coordinates that make a potential line
    inline static uint64_t bitfor(uint8_t x, uint8_t y) { return static_cast<uint64_t>(1) << (x+y*7); }  // the bit that represents a single xy point in the bitmasks
    inline static uint64_t mask_from_coords(const coordset& crds) {  // creating 4 point masks from the line coordinates
        return  (bitfor(crds[0][0], crds[0][1])) |
                (bitfor(crds[1][0], crds[1][1])) |
                (bitfor(crds[2][0], crds[2][1])) |
                (bitfor(crds[3][0], crds[3][1]));
    };
    inline static uint64_t mask_translate(uint64_t mask, uint8_t x, uint8_t y) {return mask << (x+7*y);}  // geometric translation of the masks (by delta)
    static constexpr coordset horizontal_coordinates { {0, 0}, {1, 0}, {2, 0}, {3, 0} };
    static constexpr coordset vertical_coordinates { {0, 0}, {0, 1}, {0, 2}, {0, 3} };
    static constexpr coordset fslash_coordinates { {0, 0}, {1, 1}, {2, 2}, {3, 3} };
    static constexpr coordset bslash_coordinates { {3, 0}, {2, 1}, {1, 2}, {0, 3} };
};

#endif /* board_hpp */
