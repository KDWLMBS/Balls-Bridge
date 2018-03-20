#include "motor.hpp"
#include <iostream>

#define MIN_POSITION (-1000)
#define MAX_POSITION 1000

Motor::Motor(int _index) {
    std::cout << "creating motor " << _index << std::endl;

    //the number of the motor, useful for debugging
    index = _index;

    //the current position
    position = 0;

    //the position we want to go to
    target = 0;

    //the bit-index of the enable bit
    enableBit = _index * 2;

    //the bit-index of the direction bit
    directionBit = _index * 2 + 1;
}

void Motor::debug() {
    std::cout << "position: " << position << std::endl;
    std::cout << "target: " << target << std::endl;
}

void Motor::drive(uint64_t *drive) {
    // std::cout << index << "@" << position << "+" << target << std::endl;
    if (position != target) {
        // std::cout << "drive" << std::endl;
        //set the enable bit to 1
        *drive |= 1 << enableBit;
        //depending on if we are below or above the target position we set the direction bit
        if (position < target) {
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
