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

static const Row iso_row   = 0xffff;
static const Board iso_col = 0x000f000f000f000f;
static const Row iso_tile  = 0x000f;

static Row   collapseLeft [0x10000];
static Row   collapseRight[0x10000];
static Board collapseUp   [0x10000];
static Board collapseDown [0x10000];


void create_lookup();

inline void print_board(Board board);

inline unsigned sum_board(Board board);

inline Board transpose();

inline Board swipe(Direction dir, Board board);

unsigned inline num_empty(Board board);


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
