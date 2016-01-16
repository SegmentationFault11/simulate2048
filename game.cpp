#include "game.h"

Game::Game() {
  srand((unsigned)time(NULL));
  
  board_t tile[4];
  for (row_t val = 0; val < UINT16_MAX; ++val) {
    tile[0] = val & iso_tile;
    tile[1] = (val >> 4)  & iso_tile;
    tile[2] = (val >> 8)  & iso_tile;
    tile[3] = (val >> 12) & iso_tile;
    
    for (int i = 0; i < 4; ++i)
      if (tile[i] > 1) score_row[val] += (tile[i] - 1)*(1 << tile[i]);
    
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
  ostringstream curr;
  curr << "Score = " << get_score() << endl;
  curr << "-------------------------\n";
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      int idx = (i*4 + j)*4;
      if ((board >> idx) & iso_tile) curr << "|" << left << setw(5) << setfill(' ') << (1 << ((board >> idx) & iso_tile));
      else curr << "|" << left << setw(5) << setfill(' ') << " ";
      if (j == 3) curr << "|\n-------------------------\n";
    }
  }
#ifdef OUT
  cout << curr.str();
#endif
  os << curr.str();
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
Game::get_score() {
  return score_row[(this->board >>  0) & iso_row] +
         score_row[(this->board >> 16) & iso_row] +
         score_row[(this->board >> 32) & iso_row] +
         score_row[(this->board >> 48) & iso_row] -
         this->score_pen;
}

inline board_t
Game::swipe(Direction dir, board_t board) {
  if (dir == UP) {
    board_t new_board = 0;
    board_t tran_board = transpose(board);
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
  for (int i = 0; i < 16; ++i, board >>= 4) if (!(board & iso_tile)) empty_idx.push_back(i);
  if (empty_idx.empty()) return false;
  
  int insert_idx = empty_idx[rand()%empty_idx.size()]*4;
  board_t insert_val = (rand()%10 == 0) ? 2 : 1;
  if (insert_val == 2) this->score_pen += 4;
  board_t corr = insert_val << insert_idx;
  this->board |= corr;
  
  return true;
}

inline bool
Game::game_over() {
  if (board == swipe(UP, this->board) &&
      board == swipe(DOWN, this->board) &&
      board == swipe(LEFT, this->board) &&
      board == swipe(RIGHT, this->board)) {
    return true;
  }
  else return false;
}


void
Game::Test() {
  row_t trow = 0x1111;
  cout << "row score = " << score_row[trow] << endl;
}

void
Game::Human() {
  init_board();
  
  char input = '0';
  ofstream outFile;
  string fileName;
  cout << "Name output file: " << endl;
  getline(cin, fileName);
  outFile.open(fileName);
  
  while (!game_over()) {
    print_board();
    cout << "Which move?" << endl;
    cin >> input;
    
    if (input == 'w') {
      if (this->board == swipe(UP, this->board)) continue;
      else this->board = swipe(UP, this->board);
    }
    else if (input == 's') {
      if (this->board == swipe(DOWN, this->board)) continue;
      else this->board = swipe(DOWN, this->board);
    }
    else if (input == 'a') {
      if (this->board == swipe(LEFT, this->board)) continue;
      else this->board = swipe(LEFT, this->board);
    }
    else if (input == 'd') {
      if (this->board == swipe(RIGHT, this->board)) continue;
      else this->board = swipe(RIGHT, this->board);
    }
    else if (input == 'q') {
      cout << "quitting...\n";
      break;
    }
    else {
      cout << "Wrong input\n";
      continue;
    }
    
    insert_rand();
  }
  
  os << "GAME OVER!!\nFinal Score: " << get_score() << ", Max Tile: " << get_max_tile() << "\n";
#ifdef OUT
  cout << "GAME OVER!!\nFinal Score: " << get_score() << ", Max Tile: " << get_max_tile() << "\n";
#endif
  print_board();
  
  outFile << os.str() << flush;
  outFile.close();
}

void
Game::BruteAI() {
  
}

void
Game::AI() {
  init_board();
  
  Direction next_move;
  board_t new_board;
  while (!game_over()) {
    next_move = get_best_move(this->board);
    new_board = swipe(next_move, this->board);
    insert_rand();
    print_board();
  }  
  
  cout << "GAME OVER!!\nFinal Score: " << get_score() << ", Max Tile: " << get_max_tile() << "\n";
  print_board();
}

Direction
Game::get_best_move(board_t board) {
  Direction dir;
  
  if (board) dir = UP;
  else dir = DOWN;
  return dir;
}

