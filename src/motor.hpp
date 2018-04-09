#ifndef MOTOR_H
#define MOTOR_H

#include <cstdint>
#include <iostream>

//this really shouldn't matter
#define ISR_PER_SECOND 4000

//unit: steps
#define TARGET_TOLERANCE 10

//unit: steps / second
#define VMAX 2000

#define MIN_ISR (ISR_PER_SECOND / VMAX / 2)

//unit: seconds
#define TIME_TO_VMAX 0.25

//unit: steps
#define STEPS_TO_VMAX (VMAX * TIME_TO_VMAX / 2)

//steps from center
#define MIN_POSITION (-10000)

//steps from center
#define MAX_POSITION 10000

static void printMotorConfiguration() {
#ifdef DEBUG
    std::cout << "TARGET_TOLERANCE: " << TARGET_TOLERANCE << std::endl;
    std::cout << "VMAX            : " << VMAX << std::endl;
    std::cout << "MIN_ISR         : " << MIN_ISR << std::endl;
    std::cout << "TIME_TO_VMAX    : " << TIME_TO_VMAX << std::endl;
    std::cout << "STEPS_TO_VMAX   : " << STEPS_TO_VMAX << std::endl;
    std::cout << "MIN_POSITION    : " << MIN_POSITION << std::endl;
    std::cout << "MAX_POSITION    : " << MAX_POSITION << std::endl;
#endif
}


#define ACCELERATION_DISTANCE_FOR_VELOCITY(VELOCITY) ((VELOCITY) == 0 ? 0 : abs(round((abs(VELOCITY) / VMAX) * STEPS_TO_VMAX)))

enum State {
    //DRIVE = drive at max speed in current direction
            DRIVING, ACCELERATING, STOPPING, IDLE
};

inline const char *STATE_TO_STRING(State s) {
    switch (s) {
        case DRIVING:
            return "DRIVING";
        case ACCELERATING:
            return "ACCELERATING";
        case STOPPING:
            return "STOPPING";
        case IDLE:
            return "IDLE";
        default:
            return "[Unknown State]";
    }
}

class Motor {
public:
    int velocityAtAccelerationStart;
    int stepsSinceAccelerationStart;
    int position;
    int target;
    int pwmBit;
    int directionBit;
    State state;
    int index;
    bool direction;
    int velocity;
    bool intervalPartIsHigh;
    int intervalPartCounter;

    Motor(int index);

    void debug();

    void tick(uint64_t *index);

    void update();

    void drive(uint64_t *index);

    void setTarget(int _target);

    void setState(State state);

    int intervalPartDuration;
    bool shouldUpdate;

    float calculateAccelerationSpeed();
};

#endif
