#include "game.h"

Game::Game() {
  srand((unsigned)time(NULL));
  
  board_t tile[4];
  for (row_t val = 0; val < UINT16_MAX; ++val) {
    tile[0] = val & iso_tile;
    tile[1] = (val >> 4)  & iso_tile;
    tile[2] = (val >> 8)  & iso_tile;
    tile[3] = (val >> 12) & iso_tile;
    
    for (int i = 0; i < 3; ++i) {
      int j = 0;
      
      for (j = i + 1; j < 4; ++j) if (tile[j] != 0) break;
      
      if (j == 4) break;
      
      if (tile[i] == 0) {
        tile[i] = tile[j];
        tile[j] = 0;
        i--;
      }
      else if (tile[i] == tile[j]) {
        if(tile[i] != 0xf) tile[i]++;
        tile[j] = 0;
      }
    }
    
    row_t rev_val = (val >> 12) + ((val >> 4) & 0x00F0) + ((val << 4) & 0x0F00) + (val << 12);
    
    this->collapseLeft [val    ] = (tile[0] << 0) + (tile[1] << 4) + (tile[2] << 8) + (tile[3] << 12);
    this->collapseRight[rev_val] = (tile[3] << 0) + (tile[2] << 4) + (tile[1] << 8) + (tile[0] << 12);
    this->collapseUp   [val    ] = (tile[0] << 0) + (tile[1] << 16) + (tile[2] << 32) + (tile[3] << 48);
    this->collapseDown [rev_val] = (tile[3] << 0) + (tile[2] << 16) + (tile[1] << 32) + (tile[0] << 48);
  }
}

void
Game::init_board() {
  insert_rand();
  insert_rand();
}

inline void
Game::set_board(board_t board) {
  this->board = board;
}

inline void
Game::print_board() {
  print_board(this->board);
}

inline void
Game::print_board(board_t board) {
  cout << "Score = " << this->score << endl;
  cout << "-------------------------\n";
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      int idx = (i*4 + j)*4;
      if ((board >> idx) & iso_tile) cout << "|" << left << setw(5) << setfill(' ') << (1 << ((board >> idx) & iso_tile));
      else cout << "|" << left << setw(5) << setfill(' ') << " ";
      if (j == 3) cout << "|\n-------------------------\n";
    }
  }
}

inline board_t
Game::transpose(board_t board) {
  board_t temp = (board & 0xf0f00f0ff0f00f0fULL) | ((board & 0x0000f0f00000f0f0ULL) << 12) | ((board & 0x0f0f00000f0f0000ULL) >> 12);
  return (temp & 0xff00ff0000ff00ffULL) | ((temp & 0x00ff00ff00000000ULL) >> 24) | ((temp & 0x00000000ff00ff00ULL) << 24);
}

inline unsigned
Game::sum_board() {
  unsigned sum = (this->board >> 36) +
  (this->board >> 24) & iso_row +
  (this->board >> 12) & iso_row +
  this->board & iso_row;
  return sum;
}

inline unsigned
Game::get_max_tile() {
  board_t board = this->board;
  int max = 0;
  while (board) {
    max = std::max(max, int(board & iso_tile));
    board >>= 4;
  }
  return 1 << max;
}

inline board_t
Game::swipe(Direction dir, board_t board) {
  if (dir == UP) {
    board_t new_board = 0;
    board_t tran_board = transpose(board);
    print_board(new_board);
    new_board += board_t(this->collapseUp[tran_board & iso_row]);
    new_board += board_t(this->collapseUp[(tran_board >> 16) & iso_row]) << 4;
    new_board += board_t(this->collapseUp[(tran_board >> 32) & iso_row]) << 8;
    new_board += board_t(this->collapseUp[(tran_board >> 48) & iso_row]) << 12;
    return new_board;
  }
  else if (dir == DOWN) {
    board_t new_board = 0;
    board_t tran_board = transpose(board);
    new_board += board_t(this->collapseDown[tran_board & iso_row]);
    new_board += board_t(this->collapseDown[(tran_board >> 16) & iso_row]) << 4;
    new_board += board_t(this->collapseDown[(tran_board >> 32) & iso_row]) << 8;
    new_board += board_t(this->collapseDown[(tran_board >> 48) & iso_row]) << 12;
    return new_board;
    
  }
  else if (dir == LEFT) {
    board_t new_board = 0;
    new_board += board_t(this->collapseLeft[board & iso_row]);
    new_board += board_t(this->collapseLeft[(board >> 16) & iso_row]) << 16;
    new_board += board_t(this->collapseLeft[(board >> 32) & iso_row]) << 32;
    new_board += board_t(this->collapseLeft[(board >> 48) & iso_row]) << 48;
    return new_board;
    
  }
  else if (dir == RIGHT) {
    board_t new_board = 0;
    new_board += board_t(this->collapseRight[board & iso_row]);
    new_board += board_t(this->collapseRight[(board >> 16) & iso_row]) << 16;
    new_board += board_t(this->collapseRight[(board >> 32) & iso_row]) << 32;
    new_board += board_t(this->collapseRight[(board >> 48) & iso_row]) << 48;
    return new_board;
  }
  else {
    cerr << "Wrong direction" << endl;
    exit(0);
  }
}

inline unsigned
Game::num_empty(board_t board) {
  unsigned num = 0;
  for (int i = 0; i < 16; ++i, board >>= 4) num += ((board & iso_tile)) ? 0 : 1;
  return num;
}

inline bool
Game::insert_rand() {
  board_t board = this->board;
  vector<int> empty_idx;
  for (int i = 0; i < 16; ++i, board >>= 4) if (board & iso_tile) empty_idx.push_back(i);
  if (empty_idx.empty()) return false;
  
  int insert_idx = empty_idx[rand()%empty_idx.size()]*4;
  int insert_val = (rand()%10 == 0) ? 2 : 1;
  board_t corr = insert_val << insert_idx;
  
  this->board |= corr;
  
  return true;
}

inline bool
Game::game_over() {
  if (board == swipe(UP, board) &&
      board == swipe(DOWN, board) &&
      board == swipe(LEFT, board) &&
      board == swipe(RIGHT, board)) {
    return true;
  }
  else return false;
}


void
Game::Test() {
  set_board(0x100200000f001001ULL);
  
  print_board();
  
  this->board = swipe(DOWN, this->board);
  
  print_board();
  
  insert_rand();
  
  print_board();
}

void
Game::Human() {
  
}

void
Game::BruteAi() {
  
}

void
Game::AI() {
  init_board();
  
  
}
