#ifndef MOTOR_H
#define MOTOR_H

#include <cstdint>
#include <iostream>

//we need this in order to calculate the output frequency
#ifndef ISR_PER_SECOND
#define ISR_PER_SECOND 4000
#endif

//unit: steps
#ifndef TARGET_TOLERANCE
#define TARGET_TOLERANCE 10
#endif

//unit: steps / second
#ifndef VMAX
#define VMAX 2000
#endif

//unit: seconds
#ifndef TIME_TO_VMAX
#define TIME_TO_VMAX 0.25
#endif

//steps from center
#ifndef MIN_POSITION
#define MIN_POSITION (-10000)
#endif

//steps from center
#ifndef MAX_POSITION
#define MAX_POSITION 10000
#endif

//unit: steps
#define STEPS_TO_VMAX (VMAX * TIME_TO_VMAX / 2)

//unit: steps
#define CALCULATE_INTERVAL_PART_DURATION(VELOCITY) (ISR_PER_SECOND / (VELOCITY) / 2)

static void printMotorConfiguration() {
    std::cout << "TARGET_TOLERANCE: " << TARGET_TOLERANCE << std::endl;
    std::cout << "VMAX            : " << VMAX << std::endl;
    std::cout << "TIME_TO_VMAX    : " << TIME_TO_VMAX << std::endl;
    std::cout << "STEPS_TO_VMAX   : " << STEPS_TO_VMAX << std::endl;
    std::cout << "MIN_POSITION    : " << MIN_POSITION << std::endl;
    std::cout << "MAX_POSITION    : " << MAX_POSITION << std::endl;
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
    int intervalPartDuration;
    bool shouldUpdate;


    Motor(int index);

    void debug();

    void tick(uint64_t *index);

    void update();

    void drive(uint64_t *index);

    void setTarget(int _target);

    void setState(State state);

    float calculateAccelerationSpeed();
};

#endif
