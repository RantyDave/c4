#ifndef rate_hpp
#define rate_hpp
#include <cstdint>

struct board;

class solutions
{
public:
    static int score(const board& board, int singles, int doubles, int trebles);
    
private:
    static int score(const board& board, const char* pattern, int value);
    typedef uint8_t coordset[4][2];
    static uint64_t mask_interpret(const char* in, const coordset& direction);
    static uint64_t available_interpret(const char* in, const coordset& direction);
    inline static uint64_t mask_translate(uint64_t mask, uint8_t x, uint8_t y) {return mask << (x+7*y);}
    static constexpr coordset horizontal_coordinates { {0, 0}, {1, 0}, {2, 0}, {3, 0} };
    static constexpr coordset vertical_coordinates { {0, 0}, {0, 1}, {0, 2}, {0, 3} };
    static constexpr coordset fslash_coordinates { {0, 0}, {1, 1}, {2, 2}, {3, 3} };
    static constexpr coordset bslash_coordinates { {3, 0}, {2, 1}, {1, 2}, {0, 3} };
};


#endif /* rate_hpp */
