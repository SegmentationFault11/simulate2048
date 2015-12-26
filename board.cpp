#include "board.h"

static inline unsigned sum_board(Board board) {
  unsigned sum = (board >> 36) +
                 (board >> 24) & iso_row +
                 (board >> 12) & iso_row +
                 board & iso_row;
  return sum;
}

static inline Board
transpose(Board board) {
  Board temp = board & 0xf0f00f0ff0f00f0fULL;
  Board temp1 = board & 0x0000f0f00000f0f0ULL;
  Board temp2 = board & 0x0f0f00000f0f0000ULL;
  temp = temp | (temp1 << 12) | (temp2 >> 12);
  Board temp0 = temp & 0xff00ff0000ff00ffULL;
  temp1 = temp & 0x0011001100000000ULL;
  temp2 = temp & 0x0000000011001100ULL;
  return temp0 | (temp1 << 24) | (temp2 >> 24);
}

static inline Board
swipe(Direction dir, Board board) {
  if (dir == UP) {
    board = transpose(board);
  }
  else if (dir == DOWN) {
    board = transpose(board);
    
  }
  else if (dir == LEFT) {
    
  }
  else if (dir == RIGHT) {
    
  }
  else {
    cerr << "Wrong direction" << endl;
    exit(0);
  }
  return board;
}

static unsigned inline
num_empty(Board board) {
  unsigned num = 0;
  for (int i = 16; i > 0; --i, board >>= 4) {
    num += ((board & 0xfULL)) ? 0 : 1;
  }
  return num;
}
