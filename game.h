#ifndef board_h
#define board_h

#include <iostream>
#include <cstdio>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <bitset>


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
  unsigned score_row    [0x10000];
  row_t    collapseLeft [0x10000];
  row_t    collapseRight[0x10000];
  board_t  collapseUp   [0x10000];
  board_t  collapseDown [0x10000];
  
//----------------------OUTPUT STREAM----------------------
  ostringstream os;
  
//--------------------RUNTIME VARIABLES--------------------
  unsigned search_depth  = 3;
  unsigned current_depth = 0;
 
public:
//------------------------VARIABLES------------------------
  board_t board = 0;
  unsigned score_pen = 0;
  unsigned moves = 0;
  
//----------------------CONSTRUCTORS-----------------------
  Game();
  
  Game(board_t board);
  
//--------------------BOARD OPERATIONS---------------------
  void init_board();
  
  void set_board(board_t current_board);
  
  void print_board();
  
  void print_board(board_t current_board);
  
  board_t transpose(board_t current_board);
  
  unsigned sum_board();
  
  unsigned get_max_tile();
  
  unsigned get_score();
  
  int num_unique();
  
  board_t swipe(Direction dir, board_t current_board);
  
  unsigned num_empty(board_t current_board);
  
  bool insert_rand();
  
  bool game_over();
  
  void print_fin();

//------------------------SIMULATORS------------------------
  void Test();
  
  void Human();
  
  void BruteAI();
  
  void AI();

//------------------------SIMULATORS------------------------
	Direction get_move(board_t current_board);
  
  double score_swipe(Direction dir, board_t current_board, int depth);
  
};
#endif
