#include "motor.hpp"
#include "fixed-speed-motor.hpp"
#include <vector>

#ifndef Controller_H
#define Controller_H

typedef FixedSpeedMotor ControllerMotor;


class Controller {
public:
    std::vector<ControllerMotor> motors;

    Controller(int motorStartIndex, int motorCount);

    void drive(uint64_t *data);
};

#endif
