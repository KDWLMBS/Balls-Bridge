#include "motor.hpp"
#include <iostream>

Motor::Motor(unsigned int index) {
  std::cout << "creating motor " << index << std::endl;
  this->position = 0;
  this->target = 0;
  this->enableBit = index * 2;
  this->directionBit = index * 2 + 1;
  this->index = index;
}

const void Motor::drive(unsigned long *drive) {
  if(this->position != this->target) {
    *drive &= ~(1 << this->enableBit);
  } else {
    *drive |= 1 << this->enableBit;
  }
}
