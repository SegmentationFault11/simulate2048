#ifndef Board_h
#define Board_h

#include <cstdio>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <sstream>

#include "rand.cpp"

using namespace std;

typedef uint64_t Board;
typedef uint16_t Row;

typedef enum direction_t {UP, DOWN, LEFT, RIGHT} Direction;

static const iso_row = 0x000000000000ffff;
static const iso_col = 0x000f000f000f000f;

static row_t   collapseLeft [0x10000];
static row_t   collapseRight[0x10000];
static board_t collapseUp   [0x10000];
static board_t collapseDown [0x10000];


static void create_lookup();

static inline unsigned sum_board(Board board);

static inline Board transpose();

static inline Board swipe(Direction dir, Board board);

static unsigned inline num_empty(Board board);


//If board == 0x123456789abcdef
//
//BOARD:
//---------------------
//| f  | e  | d  | c  |
//---------------------
//| b  | a  | 9  | 8  |
//---------------------
//| 7  | 6  | 5  | 4  |
//---------------------
//| 3  | 2  | 1  | 0  |
//---------------------
#endif /* Board_hpp */
