# c4
Another connect-4 solver. Probably not the best. Purely analytical, none of your fancy pants reinforced learning stuff. Also, get off my lawn.

You do need to compile it with C++17 set, but it'll go just fine with the stock libc++.

## cool things
c4 builds a big (7 layer) tree of possible moves, then prunes and extends the tree as moves are committed. The basic rule is that it will attempted to head for the greatest density of "our" victories without passing through any of "their" victories. Scoring is by giving INT_MAX to a winning result, then dividing by sixteen for each layer and adding to a total. The dividing is so a huge pile of "our" victories cannot be prioritised if it passes through a "their" victory.

The board state as held in the cache is a 64 bit number representing which holes are filled, another showing which colour they are. This means one can make a 64 bit mask describing any set of holes on the board and apply the mask onto the board to sample just those holes. So we can get, for instance, a stupid fast assessment of whether or not any four holes have been filled. There's als an 8x8bit array holding the height of each column (and a running total of the maximum) to rapidly eliminate moves that can't be made because the column is full, or shouldn't be assessed because there aren't enough pieces in the board yet. (ie there are no vertical results on a board whose maximum column height is three)
