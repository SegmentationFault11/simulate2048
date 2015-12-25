#ifndef Board_h
#define Board_h

#include <cstdio>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <sstream>

#include "rand.cpp"

using namespace std;

enum Direction {UP, DOWN, LEFT, RIGHT};

//TABLE:
//---------------------
//| 0  | 1  | 2  | 3  |
//---------------------
//| 4  | 5  | 6  | 7  |
//---------------------
//| 8  | 9  | 10 | 11 |
//---------------------
//| 12 | 13 | 14 | 15 |
//---------------------

class Board {
public:
  uint64_t board;
  
  unsigned long score;
  
  unsigned fill;
  
  //Constructor
  Board();
  
  //Add a random tile (2 or 4) to the board
  void inline addTile();
  
  //Initialises the board with 2 tiles filled
  void boardInit();
  
  //Get the current state of the board
  ostringstream inline boardState();
  
  //Print the board
  void printBoard();
  
  //Manually set value to a tile in the board
  void setTile(int pos, int val);
  void setTile(int row, int column, int val);
  
  //Simulates the swiping
  bool swipe(Direction dir);
  
  //Check if it's game over
  bool checkEnd();
  
  //Get the current score
  unsigned long getScore();
  
  //Generate an ok random number fast
  unsigned long 
};

#endif /* Board_hpp */
