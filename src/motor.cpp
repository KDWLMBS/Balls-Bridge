#include "motor.hpp"
#include <iostream>

#define TARGET_TOLERANCE 10

Motor::Motor(int _index) {
    std::cout << "creating motor " << _index << std::endl;

    //the number of the motor, useful for debugging
    index = _index;

    //the current position
    position = 0;

    //the position we want to go to
    target = 0;

    state = State::IDLE;

    velocity = 0;

    isHigh = false;

    intervalPartIndex = 0;

    //the bit-index of the pwm bit
    pwmBit = _index * 2;

    //the bit-index of the direction bit
    directionBit = _index * 2 + 1;
}

void Motor::debug() {
    std::cout << "position: " << position << std::endl;
    std::cout << "target: " << target << std::endl;
}

void Motor::setTarget(int _target) {
    target = _target;
}

void Motor::tick(uint64_t *data) {
    update();
    if (state != State::IDLE) {
        drive(data);
    }

}

void Motor::update() {
    //check with a little tolerance if we are at the target
    if (target - TARGET_TOLERANCE < position && target + TARGET_TOLERANCE > position) {
        if (velocity == 0) {
            state = State::IDLE;
        } else {
            state = State::STOPPING;
        }
    }
}

void Motor::drive(uint64_t *data) {

}
