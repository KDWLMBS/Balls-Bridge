#include "motor.hpp"
#include <vector>

#ifndef Controller_H
#define Controller_H

class Controller {
public:
    std::vector<Motor> motors;

    Controller(int motorStartIndex, int motorCount);

    void drive(uint64_t *data);
};

#endif
