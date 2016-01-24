#include "game.h"

Game::Game() {
  srand((unsigned)time(NULL));
  
  board_t tile[4];
  for (unsigned val = 0; val < 0x10000; ++val) {
    tile[0] = val & iso_tile;
    tile[1] = (val >> 4)  & iso_tile;
    tile[2] = (val >> 8)  & iso_tile;
    tile[3] = (val >> 12) & iso_tile;
    
    for (int i = 0; i < 4; ++i)
      if (tile[i] > 1) score_row[val] += (tile[i] - 1)*(1 << tile[i]);
    
    // Heuristic score
    float sum = 0;
    unsigned empty = 0;
    unsigned merges = 0;
    
    unsigned prev = 0;
    unsigned counter = 0;
    for (size_t i = 0; i < 4; ++i) {
      unsigned rank = (unsigned)tile[i];
      sum += pow(rank, SUM_POWER);
      if (rank == 0) {
        empty++;
      } else {
        if (prev == rank) {
          counter += rank;
        } else if (counter > 0) {
          merges += 1 + counter;
          counter = 0;
        }
        prev = rank;
      }
    }
    if (counter > 0) {
      merges += 1 + counter;
    }
    
    float monotonicity_left = 0;
    float monotonicity_right = 0;
    for (int i = 1; i < 4; ++i) {
      if (tile[i-1] > tile[i]) {
        monotonicity_left += pow(tile[i-1], MONOTONICITY_POWER) - pow(tile[i], MONOTONICITY_POWER);
      } else {
        monotonicity_right += pow(tile[i], MONOTONICITY_POWER) - pow(tile[i-1], MONOTONICITY_POWER);
      }
    }
    
    heuri_row[val] = LOST_PENALTY +
    								 EMPTY_WEIGHT * empty +
                     MERGES_WEIGHT * merges -
                     MONOTONICITY_WEIGHT * std::min(monotonicity_left, monotonicity_right) -
                     SUM_WEIGHT * sum;
    
    for (size_t i = 0; i < 3; ++i) {
      size_t j = 0;
      
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

inline int
Game::num_unique() {
  uint16_t present = 0;
  for (board_t current_board = this->board; current_board; current_board >>= 4)
    present |= 1 << (current_board & iso_tile);
  
  int count;
  for (count = 0; present; ++count) present &= present - 1;
  return std::max(count-2, 3);
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
Game::get_empty(board_t current_board) {
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
  string fileName = "out.txt";
//  cout << "Name output file: " << endl;
//  getline(cin, fileName);
//  outFile.open(fileName);
  
  while (!game_over()) {
    print_board();
    execute_best_move(this->board);
    ++this->moves;
    insert_rand();
  }  
  
  print_fin();
  
  outFile << os.str() << flush;
  outFile.close();
}

inline void
Game::execute_best_move(board_t current_board) {
  Direction best_move;
  
  this->search_depth = std::min(std::max(num_unique()-2, 3), 8);
  
  float up_score = 0;
  float down_score = 0;
  float left_score = 0;
  float right_score = 0;
  
  if (current_board != swipe(UP, current_board))
    up_score = expect(swipe(UP, current_board), 1);
  if (current_board != swipe(DOWN, current_board))
    down_score  = expect(swipe(DOWN, current_board), 1);
  if (current_board != swipe(LEFT, current_board))
    left_score  = expect(swipe(LEFT, current_board), 1);
  if (current_board != swipe(RIGHT, current_board))
    right_score = expect(swipe(RIGHT, current_board), 1);
  
  float best_score = std::max(up_score, down_score);
  best_score = std::max(best_score, left_score);
  best_score = std::max(best_score, right_score);
  
  if (best_score == up_score) best_move = UP;
  else if (best_score == down_score) best_move = DOWN;
  else if (best_score == left_score) best_move = LEFT;
  else best_move = RIGHT;
  
  this->board = swipe(best_move, this->board);
  unsigned max_tile = get_max_tile();
  if (max_tile > current_max_tile) this->current_max_tile = max_tile;
  this->look_up.clear();
}

inline float
Game::expect(board_t current_board, float prob) {
  if (prob < 0.0001 || this->search_depth < this->current_depth) {
    return score_board(current_board);
  }
  
  const unordered_map<board_t, pair<uint8_t, float>>::iterator& it = this->look_up.find(current_board);
  if (it != look_up.end()) return it->second.second;
  
  unsigned num_empty = get_empty(current_board);
  prob /= num_empty;
  
  float score = 0.0;
  board_t new_board = current_board;
  board_t tile = 1;
  while (tile) {
    if (!(new_board & iso_tile)) {
      score += imax(current_board |  tile      , prob*0.9)*0.9;
      score += imax(current_board | (tile << 1), prob*0.1)*0.1;
    }
    new_board >>= 4;
    tile <<= 4;
  }
  score /= num_empty;

  this->look_up.insert({current_board, {this->current_depth, score}});
  
  return score;
}

inline float
Game::imax(board_t current_board, float prob) {
  float best = 0.0;
  ++this->current_depth;
  
  Direction move;
  for (int dirInt = 0; dirInt < 4; ++dirInt) {
    move = static_cast<direction_t>(dirInt);
    board_t new_board = swipe(move, current_board);
    
    if (current_board != new_board) {
      best = std::max(best, expect(new_board, prob));
    }
  }
  --this->current_depth;
  
  return best;
}

inline float
Game::score_board(board_t current_board) {
  float score = heuri_row[(current_board >>  0) & iso_row] +
                heuri_row[(current_board >> 16) & iso_row] +
                heuri_row[(current_board >> 32) & iso_row] +
                heuri_row[(current_board >> 48) & iso_row] +
                heuri_row[(transpose(current_board) >>  0) & iso_row] +
                heuri_row[(transpose(current_board) >> 16) & iso_row] +
                heuri_row[(transpose(current_board) >> 32) & iso_row] +
                heuri_row[(transpose(current_board) >> 48) & iso_row];
  return score;
}

