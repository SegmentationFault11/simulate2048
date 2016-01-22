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
Game::set_board(board_t current_board) {
  this->board = current_board;
}

inline void
Game::print_board() {
  print_board(this->board);
}

inline void
Game::print_board(board_t current_board) {
  ostringstream curr;
  curr << "Score = " << get_score() << endl;
  curr << "-------------------------\n";
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      int idx = (i*4 + j)*4;
      if ((current_board >> idx) & iso_tile)
        curr << "|" << left << setw(5) << setfill(' ')
             << (1 << ((current_board >> idx) & iso_tile));
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
Game::transpose(board_t current_board) {
  board_t temp = (current_board & 0xf0f00f0ff0f00f0fULL) |
                 ((current_board & 0x0000f0f00000f0f0ULL) << 12) |
                 ((current_board & 0x0f0f00000f0f0000ULL) >> 12);
  return (temp & 0xff00ff0000ff00ffULL) |
         ((temp & 0x00ff00ff00000000ULL) >> 24) |
         ((temp & 0x00000000ff00ff00ULL) << 24);
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
  board_t current_board = this->board;
  int max = 0;
  while (current_board) {
    max = std::max(max, int(current_board & iso_tile));
    current_board >>= 4;
  }
  return 1 << max;
}

inline unsigned
Game::get_score() {
  return score_row[(this->board >>  0) & iso_row] +
         score_row[(this->board >> 16) & iso_row] +
         score_row[(this->board >> 32) & iso_row] +
         score_row[(this->board >> 48) & iso_row] -
         this->score_pen;
}

int
Game::num_unique() {
  uint16_t present = 0;
  for (board_t current_board = this->board; current_board; current_board >>= 4)
    present |= 1 << (current_board & iso_tile);
  
  int count;
  for (count = 0; present; ++count) present &= present - 1;
  return --count;
}

inline board_t
Game::swipe(Direction dir, board_t current_board) {
  if (dir == UP) {
    board_t new_board = 0;
    board_t tran_board = transpose(current_board);
    new_board += board_t(this->collapseUp[tran_board & iso_row]);
    new_board += board_t(this->collapseUp[(tran_board >> 16) & iso_row]) << 4;
    new_board += board_t(this->collapseUp[(tran_board >> 32) & iso_row]) << 8;
    new_board += board_t(this->collapseUp[(tran_board >> 48) & iso_row]) << 12;
    return new_board;
  }
  else if (dir == DOWN) {
    board_t new_board = 0;
    board_t tran_board = transpose(current_board);
    new_board += board_t(this->collapseDown[tran_board & iso_row]);
    new_board += board_t(this->collapseDown[(tran_board >> 16) & iso_row]) << 4;
    new_board += board_t(this->collapseDown[(tran_board >> 32) & iso_row]) << 8;
    new_board += board_t(this->collapseDown[(tran_board >> 48) & iso_row]) << 12;
    return new_board;
    
  }
  else if (dir == LEFT) {
    board_t new_board = 0;
    new_board += board_t(this->collapseLeft[current_board & iso_row]);
    new_board += board_t(this->collapseLeft[(current_board >> 16) & iso_row]) << 16;
    new_board += board_t(this->collapseLeft[(current_board >> 32) & iso_row]) << 32;
    new_board += board_t(this->collapseLeft[(current_board >> 48) & iso_row]) << 48;
    return new_board;
    
  }
  else if (dir == RIGHT) {
    board_t new_board = 0;
    new_board += board_t(this->collapseRight[current_board & iso_row]);
    new_board += board_t(this->collapseRight[(current_board >> 16) & iso_row]) << 16;
    new_board += board_t(this->collapseRight[(current_board >> 32) & iso_row]) << 32;
    new_board += board_t(this->collapseRight[(current_board >> 48) & iso_row]) << 48;
    return new_board;
  }
  else {
    cerr << "Wrong direction" << endl;
    exit(0);
  }
}

inline unsigned
Game::num_empty(board_t current_board) {
  unsigned num = 0;
  for (int i = 0; i < 16; ++i, current_board >>= 4) num += ((current_board & iso_tile)) ? 0 : 1;
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

inline void
Game::print_fin() {
  os << "\n\n!!GAME OVER!!\nFinal Score: " << get_score() << "\nMax Tile: "
     << get_max_tile() << "\nIn " << this->moves << " moves\n";
#ifdef OUT
  cout << "\n\n!!GAME OVER!!\nFinal Score: " << get_score() << "\nMax Tile: "
       << get_max_tile() << "\nIn " << this->moves << " moves\n";
#endif
  print_board();
}

void
Game::Test() {
  set_board(0x0001020040005000);
  
  print_board();
  
  cout << "num unique " << num_unique() << endl;
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
    
    ++this->moves;
    insert_rand();
  }
  
  print_fin();
  
  outFile << os.str() << flush;
  outFile.close();
}

void
Game::BruteAI() {
  init_board();
  
  ofstream outFile;
  string fileName;
  cout << "Name output file: " << endl;
  getline(cin, fileName);
  outFile.open(fileName);
  
  while (!game_over()) {
    print_board();
    if (this->board != swipe(DOWN, this->board))
      this->board = swipe(DOWN, this->board);
    else if (this->board != swipe(RIGHT, this->board))
      this->board = swipe(RIGHT, this->board);
    else if (this->board != swipe(LEFT, this->board))
      this->board = swipe(LEFT, this->board);
    else if (this->board != swipe(UP, this->board))
      this->board = swipe(UP, this->board);
    
    ++this->moves;
    insert_rand();
  }
  
  print_fin();
  
  outFile << os.str() << flush;
  outFile.close();
}

void
Game::AI() {
  init_board();
  
  ofstream outFile;
  string fileName;
  cout << "Name output file: " << endl;
  getline(cin, fileName);
  outFile.open(fileName);
  
  Direction next_move;
  while (!game_over()) {
    print_board();
    next_move = get_move(this->board);
    this->board = swipe(next_move, this->board);
    ++this->moves;
    insert_rand();
  }  
  
  print_fin();
  
  outFile << os.str() << flush;
  outFile.close();
}

Direction
Game::get_move(board_t current_board) {
  Direction best_move = UP;
  
  this->search_depth = std::min(std::max(num_unique(), 3), 9);
  
  cout << "search_depth = " << this->search_depth << endl;
  
  double up_score = 0;
  double down_score = 0;
  double left_score = 0;
  double right_score = 0;
  
  if (current_board != swipe(UP, current_board)) up_score = expect(UP, current_board);
  if (current_board != swipe(DOWN, current_board)) down_score  = expect(DOWN, current_board);
  if (current_board != swipe(LEFT, current_board)) left_score  = expect(LEFT, current_board);
  if (current_board != swipe(RIGHT, current_board)) right_score = expect(RIGHT, current_board);
  
  if (down_score > up_score) best_move = DOWN;
  if (left_score > down_score) best_move = LEFT;
  if (right_score > left_score) best_move = RIGHT;
  
  return best_move;
}

double inline
Game::expect(board_t current_board, double prob) {
  if (prob < 0.001 || this->current_depth >= this->search_depth)
    return score_board(current_board);
  
  const unordered_map<board_t, pair<uint8_t, double>>::iterator& it = this->look_up.find(current_board);
  if (it != look_up.end()) return it->second.second;
  
  unsigned num_open = num_empty(current_board);
  prob /= num_open;
  
  double score = 0.0;
  board_t tmp = current_board;
  board_t tile = 1;
  while (tile) {
    if ((tmp & iso_tile) == 0) {
      score += imax(current_board |  tile      , prob*0.9)*0.9;
      score += imax(current_board | (tile << 1), prob*0.1)*0.1;
    }
    tmp >>= 4;
    tile <<= 4;
  }
  score /= num_open;
  
  this->look_up.insert({current_board, {this->current_depth, score}});
  
  return score;
}

double inline
Game::imax(board_t current_board, double prob) {
  double best = 0.0;
  this->current_depth++;
  Direction move;
  for (int dirInt = 0; dirInt < 4; ++dirInt) {
    move = static_cast<direction_t>(dirInt);
    board_t new_board = swipe(move, current_board);
    
    if (current_board != new_board) {
      best = std::max(best, expect(new_board, prob));
    }
  }
  this->current_depth--;
  
  return best;
}

double inline
Game::score_board(board_t current_board) {
  return current_board;
}


