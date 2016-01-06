#include "board.h"

void
create_lookup() {
  for (unsigned val = 1; val < 0x10000; ++val) {
    unsigned tile[4];
    tile[0] = val & 0xf;
    tile[1] = (val >> 4)  & 0xf;
    tile[2] = (val >> 8)  & 0xf;
    tile[3] = (val >> 12) & 0xf;
    
    for (unsigned i = 0; i < 4; ++i) {
      while (tile[i] == 0) {
        tile[i] = tile[i + 1];
        tile[i + 1] = tile[i + 2];
        tile[i + 2] = tile[i + 3];
        tile[i + 3] = 0;
      }
      
      while (tile[i + 1] == 0 && (tile[i + 2] || tile[i + 3])) {
        tile[i + 1] = tile[i + 2];
        tile[i + 2] = tile[i + 3];
        tile[i + 3] = 0;
      }
      
      while (tile[i + 2] == 0 && tile[i + 3]) {
        tile[i + 2] = tile[i + 3];
        tile[i + 3] = 0;
      }
      
      if (tile[i] == tile[i + 1] && tile[i]) {
        ++tile[i];
        tile[i + 1] = tile[i + 2];
        tile[i + 2] = tile[i + 3];
        tile[i + 3] = 0;
      }
      
      if (tile[i + 1] == tile[i + 2] && tile[i + 1]) {
        ++tile[i + 1];
        tile[i + 2] = tile[i + 3];
        tile[i + 3] = 0;
      }
      
      if (tile[i + 2] == tile[i + 3] && tile[i + 2]) {
        ++tile[i + 2];
        tile[i + 3] = 0;
      }
    }
    Row collapse_left  = tile[0] + (tile [1] << 4) + (tile [2] << 8) + (tile [3] << 12);
    Row collapse_right = tile[3] + (tile [2] << 4) + (tile [1] << 8) + (tile [0] << 12);
    
    unsigned rev_val = (val >> 12) | ((val >> 4) & 0x00F0)  | ((val << 4) & 0x0F00) | (val << 12);
    
    collapseLeft [val] = collapse_left;
    collapseRight[rev_val] = collapse_right;
  }
}

inline void
print_board(Board board) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      int idx = i*4 + j;
      if (board >> idx) cout << "|" << left << setw(5) << setfill(' ') << (board >> idx);
      else cout << "|" << left << setw(5) << setfill(' ') << " ";
      if (j == 3) cout << "|\n-------------------------\n";
    }
  }
}

inline unsigned
sum_board(Board board) {
  unsigned sum = (board >> 36) +
                 (board >> 24) & iso_row +
                 (board >> 12) & iso_row +
                 board & iso_row;
  return sum;
}

inline Board
transpose(Board board) {
  Board temp = (board & 0xf0f00f0ff0f00f0f) | ((board & 0x0000f0f00000f0f0) << 12) | ((board & 0x0f0f00000f0f0000) >> 12);
  return (temp & 0xff00ff0000ff00ff) | ((temp & 0x0011001100000000) << 24) | ((temp & 0x0000000011001100) >> 24);
}

inline Board
swipe(Direction dir, Board board) {
  if (dir == UP) {
    Board new_board = transpose(board);
    return new_board;
  }
  else if (dir == DOWN) {
    Board new_board = transpose(board);
    return new_board;
    
  }
  else if (dir == LEFT) {
    Board new_board = 0;
    new_board += Board(collapseLeft[board & iso_row]);
    new_board += Board(collapseLeft[(board >> 16) & iso_row]) << 16;
    new_board += Board(collapseLeft[(board >> 32) & iso_row]) << 32;
    new_board += Board(collapseLeft[(board >> 48) & iso_row]) << 48;
    return new_board;
    
  }
  else if (dir == RIGHT) {
    Board new_board = 0;
    new_board += Board(collapseRight[board & iso_row]);
    new_board += Board(collapseRight[(board >> 16) & iso_row]) << 16;
    new_board += Board(collapseRight[(board >> 32) & iso_row]) << 32;
    new_board += Board(collapseRight[(board >> 48) & iso_row]) << 48;
    return new_board;
  }
  else {
    cerr << "Wrong direction" << endl;
    exit(0);
  }
}

unsigned inline
num_empty(Board board) {
  unsigned num = 0;
  for (int i = 16; i > 0; --i, board >>= 4) {
    num += ((board & 0xfULL)) ? 0 : 1;
  }
  return num;
}
