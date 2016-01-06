#ifndef game_h
#define game_h

#include <iostream>
#include <sstream>
#include <fstream>
#include "table.h"
#include "board.h"

using namespace std;

class Game {
private:
  Table table;
  
public:
  //Constructor
  Game();
  
  //Let humans play the game
  void playGame();
  
  //Use brute force algorithm for the game
  void bruteGame();
  
  //Use more advanced heuristics for the game
  void automateGame();
};

#endif /* game_hpp */
