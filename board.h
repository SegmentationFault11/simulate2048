#ifndef board_h
#define board_h

#include <iostream>
#include <cstdio>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <sstream>

using namespace std;


typedef uint64_t board_t;
typedef uint16_t row_t;

typedef enum direction_t {UP, DOWN, LEFT, RIGHT} Direction;


static const board_t iso_row   = 0xffff;
static const board_t iso_col = 0x000f000f000f000f;
static const row_t iso_tile  = 0x000f;

class Board {
public:
  board_t board = 0;
  unsigned score = 0;
  
  row_t   collapseLeft [0x10000];
  row_t   collapseRight[0x10000];
  board_t collapseUp   [0x10000];
  board_t collapseDown [0x10000];
  
  Board();
  
  void set_board(board_t board);
  
  unsigned get_score();
  
  void print_board();
  
  void print_board(board_t board);
  
  board_t transpose(board_t board);
  
  unsigned sum_board();
  
  unsigned get_max_tile();
  
  board_t swipe(Direction dir, board_t board);
  
  unsigned num_empty(board_t board);
};
#endif









//#ifndef Board_h
//#define Board_h
//
//#include <cstdio>
//#include <vector>
//#include <iomanip>
//#include <cstdlib>
//#include <sstream>
//
//#include "rand.cpp"
//
//using namespace std;
//
//typedef uint64_t Board;
//typedef uint16_t Row;
//
//typedef enum direction_t {UP, DOWN, LEFT, RIGHT} Direction;
//
//static const Row iso_row   = 0xffff;
//static const Board iso_col = 0x000f000f000f000f;
//static const Row iso_tile  = 0x000f;
//
//static Row   collapseLeft [0x10000];
//static Row   collapseRight[0x10000];
//static Board collapseUp   [0x10000];
//static Board collapseDown [0x10000];
//
//
//void create_lookup();
//
//void print_board(Board board);
//
//unsigned sum_board(Board board);
//
//Board transpose();
//
//Board swipe(Direction dir, Board board);
//
//unsigned inline num_empty(Board board);
//
//
////If board == 0x123456789abcdef
////
////BOARD:
////---------------------
////| f  | e  | d  | c  |
////---------------------
////| b  | a  | 9  | 8  |
////---------------------
////| 7  | 6  | 5  | 4  |
////---------------------
////| 3  | 2  | 1  | 0  |
////---------------------
//#endif /* Board_hpp */



