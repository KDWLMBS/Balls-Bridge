#include "controller.hpp"

Controller::Controller(int motorStartIndex, int motorCount) {
  for(int i = motorStartIndex; i < motorStartIndex + motorCount; i++) {
    Motor motor(i);
    motors.push_back(motor);
  }
}

void Controller::drive(unsigned long *data, int next) {
  for(int i = 0; i < motors.size(); i++) {
    motors[0].drive(data, next);
  }
}
