#include "motor.hpp"
#include <list>
#include <vector>

#ifndef Controller_H
#define Controller_H

#define MIN_POSITION -1000
#define MAX_POSITION 1000

class Controller {
public:
    int motorCount;
    std::list<Motor> motors;
//public:
  Controller(int motorStartIndex, int motorCount);
  const void drive(unsigned long *data);
};

#endif
