#ifndef game_hpp
#define game_hpp

#include <stdio.h>
#include <sstream>
#include <fstream>
#include "table.h"
#include "board.h"

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
