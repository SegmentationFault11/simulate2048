#ifndef board_h
#define board_h

#include <iostream>
#include <cstdio>
#include <vector>
#include <deque>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <bitset>
#include <cstdint>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>


using namespace std;

// board = 0x0123456789abcdef
// -----------------
// | f | e | d | c |
// -----------------
// | b | a | 9 | 8 |
// -----------------
// | 7 | 6 | 5 | 4 |
// -----------------
// | 3 | 2 | 1 | 0 |
// -----------------

static const float LOST_PENALTY = 200000.0;
static const float MONOTONICITY_POWER = 4.0;
static const float MONOTONICITY_WEIGHT = 47.0;
static const float SUM_POWER = 3.5;
static const float SUM_WEIGHT = 11.0;
static const float MERGES_WEIGHT = 700.0;
static const float EMPTY_WEIGHT = 270.0;

static const uint8_t LOOK_UP_DEPTH = 5;

typedef uint64_t board_t;
typedef uint16_t row_t;
typedef unordered_map<board_t, pair<uint8_t, float>> lookup_t;

typedef enum direction_t {UP, DOWN, LEFT, RIGHT} Direction;

static const board_t iso_row   = 0xffff;
static const board_t iso_col = 0x000f000f000f000f;
static const row_t iso_tile  = 0x000f;

class Game {
private:
//----------------------LOOKUP TABLES----------------------
  unsigned score_row    [0x10000];
  float    heuri_row    [0x10000];
  row_t    collapseLeft [0x10000];
  row_t    collapseRight[0x10000];
  board_t  collapseUp   [0x10000];
  board_t  collapseDown [0x10000];
  
//----------------------OUTPUT STREAM----------------------
  ostringstream os;
  
//--------------------RUNTIME VARIABLES--------------------
  uint8_t search_depth  = 3;
  uint8_t current_depth = 0;
  unsigned current_max_tile = 0;
 
public:
//------------------------VARIABLES------------------------
  board_t board = 0;
  unsigned score_pen = 0;
  unsigned moves = 0;
  
  deque<lookup_t*> look_up;
  
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
  
  board_t swipe(direction_t dir, board_t current_board);
  
  unsigned get_empty(board_t current_board);
  
  bool insert_rand();
  
  bool game_over();
  
  void print_fin();

//------------------------SIMULATORS------------------------
  void Test();
  
  void Human();
  
  void BruteAI();
  
  void AI();

//------------------------HEURISTICS------------------------
	void execute_best_move(board_t current_board);
  
  float expect(board_t current_board, float prob);
  
  float imax(board_t current_board, float prob);
  
  float score_board(board_t current_board);

//----------------------THREAD_WRAPPER----------------------
  void wrapper(board_t current_board, float score);
  
};
#endif
