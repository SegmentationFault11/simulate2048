#include "board.h"

Board::Board() {
  unsigned tile[4];
  for (row_t val = 0; val < UINT16_MAX; ++val) {
    tile[0] = val & iso_tile;
    tile[1] = (val >> 4)  & iso_tile;
    tile[2] = (val >> 8)  & iso_tile;
    tile[3] = (val >> 12) & iso_tile;
    
    //cout << "strt " << val << endl;
    
    for (int i = 0; i < 3; ++i) {
      int j;
      for (j = i + 1; j < 4; ++j) {
        if (tile[j] != 0) break;
      }
      if (j == 4) break; // no more tiles to the right
      
      if (tile[i] == 0) {
        tile[i] = tile[j];
        tile[j] = 0;
        i--; // retry this entry
      } else if (tile[i] == tile[j]) {
        if(tile[i] != 0xf) {
          /* Pretend that 32768 + 32768 = 32768 (representational limit). */
          tile[i]++;
        }
        tile[j] = 0;
      }
    }
    
    //cout << "mid " << val << endl;
    
    row_t collapse_left  = tile[0] + (tile [1] << 4) + (tile [2] << 8) + (tile [3] << 12);
    row_t collapse_right = tile[3] + (tile [2] << 4) + (tile [1] << 8) + (tile [0] << 12);
    
    row_t rev_val = (val >> 12) + ((val >> 4) & 0x00F0) + ((val << 4) & 0x0F00) + (val << 12);
    
    this->collapseLeft [val    ] = collapse_left;
    this->collapseRight[rev_val] = collapse_right;
    this->collapseUp   [val    ] = (val | (val << 12ULL) | (tmp << 24ULL) | (tmp << 36ULL)) & COL_MASK;
    this->collapseDown [rev_val] =
    
    //cout << "finn " << val << endl;
  }
}

void
Board::set_board(board_t board) {
  this->board = board;
}

unsigned
Board::get_score() {
  return this->score;
}

void
Board::print_board() {
  print_board(this->board);
}

void
Board::print_board(board_t board) {
  cout << board << endl;
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

board_t
Board::transpose(board_t board) {
  board_t temp = (board & 0xf0f00f0ff0f00f0f) | ((board & 0x0000f0f00000f0f0) << 12) | ((board & 0x0f0f00000f0f0000) >> 12);
  return (temp & 0xff00ff0000ff00ff) | ((temp & 0x0011001100000000) << 24) | ((temp & 0x0000000011001100) >> 24);
}

unsigned
Board::sum_board() {
  unsigned sum = (this->board >> 36) +
  (this->board >> 24) & iso_row +
  (this->board >> 12) & iso_row +
  this->board & iso_row;
  return sum;
}

unsigned
Board::get_max_tile() {
  unsigned max = 0;
  return max;
}

board_t
Board::swipe(Direction dir, board_t board) {
  if (dir == UP) {
    board_t new_board = transpose(board);
    return new_board;
  }
  else if (dir == DOWN) {
    board_t new_board = transpose(board);
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

unsigned
Board::num_empty(board_t board) {
  unsigned num = 0;
  for (int i = 16; i > 0; --i, board >>= 4) {
    num += ((board & 0xfULL)) ? 0 : 1;
  }
  return num;
}



//#include "board.h"
//
//void
//create_lookup() {
//  for (unsigned val = 1; val < 0x10000; ++val) {
//    unsigned tile[4];
//    tile[0] = val & 0xf;
//    tile[1] = (val >> 4)  & 0xf;
//    tile[2] = (val >> 8)  & 0xf;
//    tile[3] = (val >> 12) & 0xf;
//    
//    for (unsigned i = 0; i < 4; ++i) {
//      while (tile[i] == 0) {
//        tile[i] = tile[i + 1];
//        tile[i + 1] = tile[i + 2];
//        tile[i + 2] = tile[i + 3];
//        tile[i + 3] = 0;
//      }
//      
//      while (tile[i + 1] == 0 && (tile[i + 2] || tile[i + 3])) {
//        tile[i + 1] = tile[i + 2];
//        tile[i + 2] = tile[i + 3];
//        tile[i + 3] = 0;
//      }
//      
//      while (tile[i + 2] == 0 && tile[i + 3]) {
//        tile[i + 2] = tile[i + 3];
//        tile[i + 3] = 0;
//      }
//      
//      if (tile[i] == tile[i + 1] && tile[i]) {
//        ++tile[i];
//        tile[i + 1] = tile[i + 2];
//        tile[i + 2] = tile[i + 3];
//        tile[i + 3] = 0;
//      }
//      
//      if (tile[i + 1] == tile[i + 2] && tile[i + 1]) {
//        ++tile[i + 1];
//        tile[i + 2] = tile[i + 3];
//        tile[i + 3] = 0;
//      }
//      
//      if (tile[i + 2] == tile[i + 3] && tile[i + 2]) {
//        ++tile[i + 2];
//        tile[i + 3] = 0;
//      }
//    }
//    Row collapse_left  = tile[0] + (tile [1] << 4) + (tile [2] << 8) + (tile [3] << 12);
//    Row collapse_right = tile[3] + (tile [2] << 4) + (tile [1] << 8) + (tile [0] << 12);
//    
//    unsigned rev_val = (val >> 12) | ((val >> 4) & 0x00F0)  | ((val << 4) & 0x0F00) | (val << 12);
//    
//    collapseLeft [val] = collapse_left;
//    collapseRight[rev_val] = collapse_right;
//  }
//}
//
//void
//print_board(Board board) {
//  for (int i = 0; i < 4; ++i) {
//    for (int j = 0; j < 4; ++j) {
//      int idx = i*4 + j;
//      if (board >> idx) cout << "|" << left << setw(5) << setfill(' ') << (board >> idx);
//      else cout << "|" << left << setw(5) << setfill(' ') << " ";
//      if (j == 3) cout << "|\n-------------------------\n";
//    }
//  }
//}
//
//unsigned
//sum_board(Board board) {
//  unsigned sum = (board >> 36) +
//  (board >> 24) & iso_row +
//  (board >> 12) & iso_row +
//  board & iso_row;
//  return sum;
//}
//
//Board
//transpose(Board board) {
//  Board temp = (board & 0xf0f00f0ff0f00f0f) | ((board & 0x0000f0f00000f0f0) << 12) | ((board & 0x0f0f00000f0f0000) >> 12);
//  return (temp & 0xff00ff0000ff00ff) | ((temp & 0x0011001100000000) << 24) | ((temp & 0x0000000011001100) >> 24);
//}
//
//Board
//swipe(Direction dir, Board board) {
//  if (dir == UP) {
//    Board new_board = transpose(board);
//    return new_board;
//  }
//  else if (dir == DOWN) {
//    Board new_board = transpose(board);
//    return new_board;
//    
//  }
//  else if (dir == LEFT) {
//    Board new_board = 0;
//    new_board += Board(collapseLeft[board & iso_row]);
//    new_board += Board(collapseLeft[(board >> 16) & iso_row]) << 16;
//    new_board += Board(collapseLeft[(board >> 32) & iso_row]) << 32;
//    new_board += Board(collapseLeft[(board >> 48) & iso_row]) << 48;
//    return new_board;
//    
//  }
//  else if (dir == RIGHT) {
//    Board new_board = 0;
//    new_board += Board(collapseRight[board & iso_row]);
//    new_board += Board(collapseRight[(board >> 16) & iso_row]) << 16;
//    new_board += Board(collapseRight[(board >> 32) & iso_row]) << 32;
//    new_board += Board(collapseRight[(board >> 48) & iso_row]) << 48;
//    return new_board;
//  }
//  else {
//    cerr << "Wrong direction" << endl;
//    exit(0);
//  }
//}
//
//unsigned
//num_empty(Board board) {
//  unsigned num = 0;
//  for (int i = 16; i > 0; --i, board >>= 4) {
//    num += ((board & 0xfULL)) ? 0 : 1;
//  }
//  return num;
//}
