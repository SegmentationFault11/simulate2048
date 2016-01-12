#include "table.h"

Table::Table() {
  score = 0;
  fill = 0;
  board.resize(16);
  for (int i = 0; i < 16; ++i) board[i] = 0;
  srand((int)time(NULL));
}

void inline
Table::addTile() {
  if (fill == 16) {
    cout << "!!GAME OVER!!\n";
    printBoard();
    return;
  }
  
  int value = 0;
  int position = 0;
  
  value = 2 + 2*(rand()%2);
  do position = rand()%16;
  while (board[position]);
  board[position] = value;
  ++fill;

  
}


void
Table::boardInit() {
  addTile();
  addTile();
}

ostringstream
Table::boardState() {
  ostringstream out;
  out << "SCORE = " << score << "\n";
  out << "-------------------------\n";
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (board[i*4 + j]) out << "|" << left << setw(5) << setfill(' ') << board[i*4 + j];
      else out << "|" << left << setw(5) << setfill(' ') << " ";
      if (j == 3) out << "|\n-------------------------\n";
    }
  }
  out << flush;
  return out;
}

void
Table::printBoard() {
  cout << boardState().str();
}

void
Table::setTile(int pos, int val) {
  board[pos] = val;
}

void
Table::setTile(int row, int column, int val) {
  board[row*4 + column] = val;
}

//Try not to read this horribly writting function
bool
Table::swipe(int dir) {
  bool changed = false;
  if (dir == 0) {
    for (int i = 0; i < 4; ++i) {
      if (board[i] == 0 && board[i + 4] == 0 && board[i + 8] == 0 && board[i + 12] == 0) continue;
      
      while (board[i] == 0) {
        changed = true;
        board[i] = board[i + 4];
        board[i + 4] = board[i + 8];
        board[i + 8] = board[i + 12];
        board[i + 12] = 0;
      }
      
      while (board[i + 4] == 0 && (board[i + 8] || board[i + 12])) {
        changed = true;
        board[i + 4] = board[i + 8];
        board[i + 8] = board[i + 12];
        board[i + 12] = 0;
      }
      
      while (board[i + 8] == 0 && board[i + 12]) {
        changed = true;
        board[i + 8] = board[i + 12];
        board[i + 12] = 0;
      }
      
      if (board[i] == board[i + 4] && board[i]) {
        changed = true;
        --fill;
        board[i] += board[i + 4];
        score += board[i];
        board[i + 4] = board[i + 8];
        board[i + 8] = board[i + 12];
        board[i + 12] = 0;
      }
      
      if (board[i + 4] == board[i + 8] && board[i + 4]) {
        changed = true;
        --fill;
        board[i + 4] += board[i + 8];
        score += board[i + 4];
        board[i + 8] = board[i + 12];
        board[i + 12] = 0;
      }
      
      if (board[i + 8] == board[i + 12] && board[i + 8]) {
        changed = true;
        --fill;
        board[i + 8] += board[i + 12];
        score += board[i + 8];
        board[i + 12] = 0;
      }
    }
  }
  if (dir == 1) {
    for (int i = 12; i < 16; ++i) {
      if (board[i] == 0 && board[i - 4] == 0 && board[i - 8] == 0 && board[i - 12] == 0) continue;
      
      //Eliminate all zeros
      while (board[i] == 0) {
        changed = true;
        board[i] = board[i - 4];
        board[i - 4] = board[i - 8];
        board[i - 8] = board[i - 12];
        board[i - 12] = 0;
      }
      
      while (board[i - 4] == 0 && (board[i - 8] || board[i - 12])) {
        changed = true;
        board[i - 4] = board[i - 8];
        board[i - 8] = board[i - 12];
        board[i - 12] = 0;
      }
      
      while (board[i - 8] == 0 && board[i - 12]) {
        changed = true;
        board[i - 8] = board[i - 12];
        board[i - 12] = 0;
      }
      
      if (board[i] == board[i - 4] && board[i]) {
        changed = true;
        --fill;
        board[i] += board[i - 4];
        score += board[i];
        board[i - 4] = board[i - 8];
        board[i - 8] = board[i - 12];
        board[i - 12] = 0;
      }
      
      if (board[i - 4] == board[i - 8] && board[i - 4]) {
        changed = true;
        --fill;
        board[i - 4] += board[i - 8];
        score += board[i - 4];
        board[i - 8] = board[i - 12];
        board[i - 12] = 0;
      }
      
      if (board[i - 8] == board[i - 12] && board[i - 8]) {
        changed = true;
        --fill;
        board[i - 8] += board[i - 12];
        score += board[i - 8];
        board[i - 12] = 0;
      }
    }
  }
  if (dir == 2) {
    for (int i = 0; i < 13; i += 4) {
      if (board[i] == 0 && board[i + 1] == 0 && board[i + 2] == 0 && board[i + 3] == 0) continue;
      
      while (board[i] == 0) {
        changed = true;
        board[i] = board[i + 1];
        board[i + 1] = board[i + 2];
        board[i + 2] = board[i + 3];
        board[i + 3] = 0;
      }
      
      while (board[i + 1] == 0 && (board[i + 2] || board[i + 3])) {
        changed = true;
        board[i + 1] = board[i + 2];
        board[i + 2] = board[i + 3];
        board[i + 3] = 0;
      }
      
      while (board[i + 2] == 0 && board[i + 3]) {
        changed = true;
        board[i + 2] = board[i + 3];
        board[i + 3] = 0;
      }
      
      if (board[i] == board[i + 1] && board[i]) {
        changed = true;
        --fill;
        board[i] += board[i + 1];
        score += board[i];
        board[i + 1] = board[i + 2];
        board[i + 2] = board[i + 3];
        board[i + 3] = 0;
      }
      
      if (board[i + 1] == board[i + 2] && board[i + 1]) {
        changed = true;
        --fill;
        board[i + 1] += board[i + 2];
        score += board[i + 1];
        board[i + 2] = board[i + 3];
        board[i + 3] = 0;
      }
      
      if (board[i + 2] == board[i + 3] && board[i + 2]) {
        changed = true;
        --fill;
        board[i + 2] += board[i + 3];
        score += board[i + 2];
        board[i + 3] = 0;
      }
    }
  }
  if (dir == 3) {
    for (int i = 3; i < 16; i += 4) {
      if (board[i] == 0 && board[i - 1] == 0 && board[i - 2] == 0 && board[i - 3] == 0) continue;
      
      while (board[i] == 0) {
        changed = true;
        board[i] = board[i - 1];
        board[i - 1] = board[i - 2];
        board[i - 2] = board[i - 3];
        board[i - 3] = 0;
      }
      
      while (board[i - 1] == 0 && (board[i - 2] || board[i - 3])) {
        changed = true;
        board[i - 1] = board[i - 2];
        board[i - 2] = board[i - 3];
        board[i - 3] = 0;
      }
      
      while (board[i - 2] == 0 && board[i - 3]) {
        changed = true;
        board[i - 2] = board[i - 3];
        board[i - 3] = 0;
      }
      
      if (board[i] == board[i - 1] && board[i]) {
        changed = true;
        --fill;
        board[i] += board[i - 1];
        score += board[i];
        board[i - 1] = board[i - 2];
        board[i - 2] = board[i - 3];
        board[i - 3] = 0;
      }
      
      if (board[i - 1] == board[i - 2] && board[i - 1]) {
        changed = true;
        --fill;
        board[i - 1] += board[i - 2];
        score += board[i - 1];
        board[i - 2] = board[i - 3];
        board[i - 3] = 0;
      }
      
      if (board[i - 2] == board[i - 3] && board[i - 2]) {
        changed = true;
        --fill;
        board[i - 2] += board[i - 3];
        score += board[i - 2];
        board[i - 3] = 0;
      }
    }
  }
  if (changed) {
    this->addTile();
    if (checkEnd()) cout << "!!!GAMEOVER!!!" << endl;
  }
  return changed;
}

bool
Table::checkEnd() {
  bool end = true;
  if (this->fill < 16) return false;
  for (int i = 0; i < 12; ++i) if (board[i] == board[i + 4]) end = false;
  for (int i = 0; i < 15; ++i) {
    if (i != 3 && i != 7 && i != 11 && board[i] == board[i + 1]) end = false;
  }
  return end;
}
