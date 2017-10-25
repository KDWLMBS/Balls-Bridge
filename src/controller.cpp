#include "controller.hpp"

Controller::Controller(int motorStartIndex, int motorCount) {
  for(int i = motorStartIndex; i < motorStartIndex + motorCount; i++) {
    Motor motor(i);
    this->motors.push_back(motor);
  }
}

const void Controller::drive(unsigned long *data, int next) {
  for (std::list<Motor>::const_iterator iterator = this->motors.begin(), end = this->motors.end(); iterator != end; ++iterator) {
    Motor motor = *iterator;
    motor.drive(data, next);
  }
}
