#include <iostream>
#include <getopt.h>
#include "game.h"

int main(int argc, char* argv[]) {
  ios_base::sync_with_stdio(false);
  
  static struct option longopts[] = {
    {"test",     no_argument      , NULL, 't'},
    {"human",    no_argument      , NULL, 'h'},
    {"brute",    no_argument      , NULL, 'b'},
    {"computer", no_argument      , NULL, 'c'},
    {0,          0,                 0,     0 }
  };
  
  char c;
  int idx;
  Game game;
  
  while((c = getopt_long(argc, argv, "hbct", longopts, &idx)) != -1)
  {
    switch(c)
    {
      case 't':
        game.Test();
        break;
      case 'h':
        game.Human();
        break;
      case 'b':
        game.BruteAI();
        break;
      case 'c':
        game.AI();
        break;
    }
  }
  
  return 0;
}
