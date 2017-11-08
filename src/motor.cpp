#include "motor.hpp"
#include <iostream>

#define MIN_POSITION -1000
#define MAX_POSITION 1000

Motor::Motor(int _index) {
  std::cout << "creating motor " << index << std::endl;
  position = 0;
  target = 0;
  enableBit = index * 2;
  directionBit = index * 2 + 1;
  index = _index;
}

void Motor::debug() {
  std::cout << "position: " << position << std::endl;
  std::cout << "target: " << target << std::endl;
}

void Motor::drive(unsigned long *drive, int next) {
  if(position != target) {
    //set the enable bit to 1
    *drive |= 1 << enableBit;
    //depending on if we are below or above the target position we set the direction bit
    if(position < target) {
      position++;
      *drive |= 1 << directionBit;
    } else {
      position--;
      *drive &= ~(1 << directionBit);
    }
  } else {
    //set the enable bit to 0
    *drive &= ~(1 << enableBit);
  }
}
