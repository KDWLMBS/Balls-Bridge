#include <cstdint>
#include <iostream>
#include <cmath>
#include "fixed-speed-motor.hpp"

FixedSpeedMotor::FixedSpeedMotor(int _index) {

    //the number of the motor, useful for debugging
    index = _index;

    //the current position
    position = 0;

    //the position we want to go to
    target = 0;

    shouldUpdate = false;

    state = State::IDLE;

    intervalPartCounter = 0;
    intervalPartDuration = INTERVAL_PART_DURATION;
    intervalPartIsHigh = true;
    direction = false;

    //the bit-index of the pwm bit
    pwmBit = _index * 2;

    //the bit-index of the direction bit
    directionBit = _index * 2 + 1;
}

void FixedSpeedMotor::tick(uint64_t *data) {
    if (shouldUpdate || state == State::IDLE) {
        update();
    }
    if (state == State::DRIVING) {
        drive(data);
    }
}

void FixedSpeedMotor::update() {
#if DEBUG
    std::cout << "update started" << std::endl;
#endif
    shouldUpdate = false;
    if (target == position) {
        state = State::IDLE;
    } else {
        intervalPartCounter = 0;
        intervalPartIsHigh = true;
        direction = target > position;
        state = State::DRIVING;
    }
}

void FixedSpeedMotor::drive(uint64_t *data) {
#if DEBUG
    std::cout << "drive started" << std::endl;
#endif
    if (intervalPartIsHigh) {
        *data |= (1 << pwmBit);
    } else {
        *data &= ~(1 << pwmBit);
    }
    if (direction) {
        *data |= (1 << directionBit);
    } else {
        *data &= ~(1 << directionBit);
    }
    intervalPartCounter++;
    if (intervalPartCounter >= intervalPartDuration) {
        if (intervalPartIsHigh) {
            //we are at the end of the high-cycle -> next time start the low-cycle
            intervalPartIsHigh = false;
            intervalPartCounter = 0;
        } else {
            //we are at the end of the low-cycle -> update again
            if (direction) {
                position++;
            } else {
                target--;
            }
#if DEBUG
            std::cout << "drive cycle end" << std::endl;
#endif
            shouldUpdate = true;
        }
    }
}
