#include <iostream>
#include "controller.hpp"

Controller::Controller(int motorStartIndex, int motorCount) {
    for (int i = motorStartIndex; i <= motorStartIndex + motorCount - 1; i++) {
        Motor motor(i);
        motors.push_back(motor);
    }
}

void Controller::drive(unsigned long *data, int next) {
    for (auto &motor : motors) {
        motor.drive(data, next);
    }
}
