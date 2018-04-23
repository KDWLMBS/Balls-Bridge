#include <iostream>
#include "controller.hpp"

Controller::Controller(int motorStartIndex, int motorCount) {
    std::cout << "creating new controller instance with " << motorCount << " motors" << std::endl;
    for (int i = motorStartIndex; i <= motorStartIndex + motorCount - 1; i++) {
        ControllerMotor motor(i);
        motors.push_back(motor);
    }
}

void Controller::drive(uint64_t *data) {
    for (auto &motor : motors) {
        motor.drive(data);
    }
}
