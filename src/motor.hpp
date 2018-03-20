#ifndef MOTOR_H
#define MOTOR_H


#include <cstdint>

class Motor {
public:
    int position;
    int target;
    int enableBit;
    int directionBit;
    int index;

    Motor(int index);

    void debug();

    void drive(uint64_t *index);
};

#endif
