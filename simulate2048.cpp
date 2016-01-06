#include <iostream>
#include <getopt.h>
#include "driver.h"

int main(int argc, char* argv[]) {
  ios_base::sync_with_stdio(true);
  
  static struct option longopts[] = {
    {"human",    no_argument      , NULL, 'h'},
    {"brute",    no_argument      , NULL, 'b'},
    {"computer", no_argument      , NULL, 'c'},
    {0,          0,                 0,     0 }
  };
  
  char c;
  int idx;
  Driver driver;
  
  while((c = getopt_long(argc, argv, "hbc", longopts, &idx)) != -1)
  {
    switch(c)
    {
      case 'h':
        driver.playGame();
        break;
      case 'b':
        driver.bruteGame();
        break;
      case 'c':
        driver.automateGame();
        break;
    }
  }
  
  return 0;
}
