#ifndef driver_h
#define driver_h

#include <iostream>
#include <sstream>
#include <fstream>
#include "table.h"
#include "game.h"

using namespace std;

class Driver {
private:
  Table table;
  
  Game game;
  
public:
  //Constructor
  Driver();
  
  //Let humans play the game
  void playGame();
  
  //Use brute force algorithm for the game
  void bruteGame();
  
  //Use more advanced heuristics for the game
  void automateGame();
};

#endif /* game_hpp */
