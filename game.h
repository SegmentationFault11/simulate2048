#ifndef board_h
#define board_h

#include <iostream>
#include <cstdio>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <algorithm>


using namespace std;


typedef uint64_t board_t;
typedef uint16_t row_t;

typedef enum direction_t {UP, DOWN, LEFT, RIGHT} Direction;

static const board_t iso_row   = 0xffff;
static const board_t iso_col = 0x000f000f000f000f;
static const row_t iso_tile  = 0x000f;

class Game {
private:
//----------------------LOOKUP TABLES----------------------
  row_t   collapseLeft [0x10000];
  row_t   collapseRight[0x10000];
  board_t collapseUp   [0x10000];
  board_t collapseDown [0x10000];
 
public:
//------------------------VARIABLES------------------------
  board_t board = 0;
  unsigned score = 0;
  
//----------------------CONSTRUCTORS-----------------------
  Game();
  
  Game(board_t board);
  
//--------------------BOARD OPERATIONS---------------------
  void init_board();
  
  void set_board(board_t board);
  
  void print_board();
  
  void print_board(board_t board);
  
  board_t transpose(board_t board);
  
  unsigned sum_board();
  
  unsigned get_max_tile();
  
  board_t swipe(Direction dir, board_t board);
  
  unsigned num_empty(board_t board);
  
  bool insert_rand();

//------------------------SIMULATORS------------------------
  void Human();
  
  void BruteAi();
  
  void AI();
};
#endif
