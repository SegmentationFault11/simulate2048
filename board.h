#ifndef Board_h
#define Board_h

#include <cstdio>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <sstream>

#include "rand.cpp"

using namespace std;

enum Direction {UP, DOWN, LEFT, RIGHT};

static const iso_row = 0x000000000000ffffULL;
static const iso_col = 0x000f000f000f000fULL;

//BOARD:
//---------------------
//| 0  | 1  | 2  | 3  |
//---------------------
//| 4  | 5  | 6  | 7  |
//---------------------
//| 8  | 9  | 10 | 11 |
//---------------------
//| 12 | 13 | 14 | 15 |
//---------------------

typedef uint64_t Board;

static inline unsigned sum_board(Board board);

static inline Board transpose();

static inline Board swipe(Direction dir, Board board);

static unsigned inline num_empty(Board board);



#endif /* Board_hpp */
