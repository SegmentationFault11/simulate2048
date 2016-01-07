#include "driver.h"

Driver::Driver() {
  table.boardInit();
  
}

void
Driver::playGame() {
  bool keepGoing = true;
  char input = '0';
  bool changed = false;
  
  table.printBoard();
  for (int step = 0; keepGoing; ++step) {
    cout << "\nWhich direction?" << endl;
    cin >> input;
    changed = false;
    if (input == 'w') {
      changed = table.swipe(UP);
      table.printBoard();
      if (table.fill == 16 && table.checkEnd()) return;
    }
    else if (input == 's') {
      changed = table.swipe(DOWN);
      table.printBoard();
      if (table.fill == 16 && table.checkEnd()) return;
    }
    else if (input == 'a') {
      changed = table.swipe(LEFT);
      table.printBoard();
      if (table.fill == 16 && table.checkEnd()) return;
    }
    else if (input == 'd') {
      changed = table.swipe(RIGHT);
      table.printBoard();
      if (table.fill == 16 && table.checkEnd()) return;
    }
    else if (input == 'q') {
      cout << "Quiting..." << endl;
      return;
    }
    else cout << "Wrong input" << endl;
  }
}

void
Driver::bruteGame() {
  ostringstream os;
  bool gameOver = false;
  
  ofstream outFile;
  string fileName;
  cout << "Name output file: " << endl;
  getline(cin, fileName);
  outFile.open(fileName);
  
  outFile << table.boardState().str();
  for (int step = 1; !gameOver; ++step) {
    if (table.swipe(DOWN)) {
      outFile << table.boardState().str();
      if (table.checkEnd()) {
        outFile << "FINAL SCORE: " << table.score
        << "\nNumber of steps: " << step
        << "\nGAMEOVER" << endl;
        return;
      }
    }
    else if (table.swipe(RIGHT)) {
      outFile << table.boardState().str();
      if (table.checkEnd()) {
        outFile << "FINAL SCORE: " << table.score
        << "\nNumber of steps: " << step
        << "\nGAMEOVER" << endl;
        return;
      }
    }
    else if (table.swipe(LEFT)) {
      outFile << table.boardState().str();
      if (table.checkEnd()) {
        outFile << "FINAL SCORE: " << table.score
        << "\nNumber of steps: " << step
        << "\nGAMEOVER" << endl;
        return;
      }
    }
    else if (table.swipe(UP)) {
      outFile << table.boardState().str();
      if (table.checkEnd()) {
        outFile << "FINAL SCORE: " << table.score
        << "\nNumber of steps: " << step
        << "\nGAMEOVER" << endl;
        return;
      }
    }
    else cout << "what?" << endl;
  }
  
  outFile.close();
}


void
Driver::automateGame() {
  game.run();
  
}
