#include "motor.hpp"

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

    stepsSinceAccelerationStart = 0;

    velocityAtAccelerationStart = 0;

    intervalPartDuration = 0;

    intervalPartIsHigh = false;

    shouldUpdate = false;

    intervalPartCounter = 0;

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
    if (state == State::IDLE || shouldUpdate) {
        update();
    }
    if (state != State::IDLE) {
        drive(data);
    }
}

float Motor::calculateAccelerationSpeed() {
    float targetSpeed;
    if (state == State::ACCELERATING) {
        targetSpeed = VMAX;
    } else if (state == State::STOPPING) {
        targetSpeed = 0;
    } else {
        //if we get here we have an error somewhere in the code
        std::cerr
                << "state is neither ACCELERATING nor STOPPING therefore cannot determine targetSpeed. something is wrong"
                << std::endl;
        targetSpeed = 0;
    }
    float deltaV = std::abs(velocityAtAccelerationStart - targetSpeed);
    //TODO use ISRs since acceleration start to determine acceleration percentage
    float accelerationPercentage = (velocity - velocityAtAccelerationStart) / deltaV;
    return (deltaV * accelerationPercentage) + velocityAtAccelerationStart;
}

void Motor::setState(State _state) {
    if (state != _state) {
#if DEBUG
        std::cout << "state switched to " << STATE_TO_STRING(_state) << std::endl;
#endif
    }
    state = _state;
    switch (_state) {
        case State::ACCELERATING:
        case State::STOPPING:
            velocityAtAccelerationStart = velocity;
            stepsSinceAccelerationStart = 0;
            velocity = calculateAccelerationSpeed();
            intervalPartCounter = 0;
            intervalPartDuration = static_cast<int>(CALCULATE_INTERVAL_PART_DURATION(velocity));
            intervalPartIsHigh = true;
            if (state == State::STOPPING) {
                direction = velocity > 0;
            } else if (state == State::ACCELERATING) {
                //we cannot check for the velocity here because we might just be starting to accelerate -> velocity would be 0 then
                direction = target > position;
            }
            break;
        case State::IDLE:
            stepsSinceAccelerationStart = 0;
            velocity = 0;
        case State::DRIVING:
            stepsSinceAccelerationStart = 0;
    }
}

void Motor::update() {
    //check with a little tolerance if we are at the target
    if (abs(target - position) <= TARGET_TOLERANCE) {
        //if we are where we want to be
        if (velocity == 0) {
#if DEBUG
            std::cout << "at target and no longer moving -> IDLE" << std::endl;
#endif
            //and we are not moving -> IDLE
            setState(State::IDLE);
        } else {
            //but we are moving -> STOP
#if DEBUG
            std::cout << "at target but still moving, velocity (" << velocity << ") -> STOPPING" << std::endl;
#endif
            setState(State::STOPPING);
        }
    } else if ((velocity > 0 && target < position) || (velocity < 0 && target > position)) {
        //if we are still moving but the wrong way
#if DEBUG
        std::cout << "wrong direction -> STOPPING" << std::endl;
        setState(State::STOPPING);
#endif
    } else if (velocity == VMAX && ACCELERATION_DISTANCE_FOR_VELOCITY(velocity) < abs(target - position)) {
        //if we are moving at VMAX and the target is futher away than the stopping distance
        setState(State::DRIVING);
    } else if (abs(target - position) <= ACCELERATION_DISTANCE_FOR_VELOCITY(velocity)) {
        //if we are moving but the target is within our acceleration distance
        setState(State::STOPPING);
#if DEBUG
        std::cout << "need to stop, distance to target: " << abs(target - position) << ", acceleration distance: "
                  << ACCELERATION_DISTANCE_FOR_VELOCITY(velocity) << std::endl;
        setState(State::STOPPING);
#endif
    } else if (abs(target - position) > ACCELERATION_DISTANCE_FOR_VELOCITY(velocity)) {
        setState(State::ACCELERATING);
    }
}

void Motor::drive(uint64_t *data) {
#if DEBUG
    std::cout << "drive started" << std::endl;
#endif
    if (state == State::ACCELERATING || state == State::STOPPING || state == State::DRIVING) {
        if (intervalPartIsHigh) {
            *data |= (1 << pwmBit);
        } else {
            *data &= ~(1 << pwmBit);
        }
        if (velocity >= 0) {
            *data |= (1 << directionBit);
        } else {
            *data &= ~(1 << directionBit);
        }
        intervalPartCounter++;
        if (intervalPartCounter == intervalPartDuration) {
            if (intervalPartIsHigh) {
                //we are at the end of the high-cycle -> next time start the low-cycle
                intervalPartIsHigh = false;
                intervalPartCounter = 0;
            } else {
                //we are at the end of the low-cycle -> update again
                stepsSinceAccelerationStart++;
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
    } else if (state == State::IDLE) {
#if DEBUG
        std::cout << "state is idle, won't do anything" << std::endl;
#endif
    }
}
