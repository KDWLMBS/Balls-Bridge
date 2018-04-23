#ifndef MOTOR_H
#define MOTOR_H

#include <cstdint>
#include <iostream>
#include <cmath>
#include <iomanip>
#include "state.hpp"

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

#ifndef VMIN
#define VMIN 200
#endif

#ifndef INTERVAL_PART_DURATION_VMAX
#define INTERVAL_PART_DURATION_VMAX (round(ISR_PER_SECOND / VMAX / 2))
#endif

#ifndef INTERVAL_PART_DURATION_VMIN
#define INTERVAL_PART_DURATION_VMIN (round(ISR_PER_SECOND / VMIN / 2))
#endif

//unit: seconds
#ifndef TIME_TO_VMAX
#define TIME_TO_VMAX 0.25
#endif

//steps from center
#ifndef MIN_POSITION
#define MIN_POSITION (-1000)
#endif

//steps from center
#ifndef MAX_POSITION
#define MAX_POSITION 1000
#endif

//unit: steps
#define STEPS_TO_VMAX (VMAX * TIME_TO_VMAX / 2)

//unit: steps
#define CALCULATE_INTERVAL_PART_DURATION(VELOCITY) round((double)(VMAX - (VELOCITY)) / VMAX * (INTERVAL_PART_DURATION_VMIN - INTERVAL_PART_DURATION_VMAX) + INTERVAL_PART_DURATION_VMAX)

#define CALCULATE_ISR_FOR_DELTAV(DELTAV) abs(round((float)(DELTAV) / VMAX * TIME_TO_VMAX * ISR_PER_SECOND))

static void printMotorConfiguration() {
    std::cout << "+-------------------------------+" << std::endl;
    std::cout << "| Motor Configuration           |" << std::endl;
    std::cout << "| TARGET_TOLERANCE = " << std::setw(10) << TARGET_TOLERANCE << " |" << std::endl;
    std::cout << "| VMAX             = " << std::setw(10) << VMAX << " |" << std::endl;
    std::cout << "| TIME_TO_VMAX     = " << std::setw(10) << TIME_TO_VMAX << " |" << std::endl;
    std::cout << "| STEPS_TO_VMAX    = " << std::setw(10) << STEPS_TO_VMAX << " |" << std::endl;
    std::cout << "| MIN_POSITION     = " << std::setw(10) << MIN_POSITION << " |" << std::endl;
    std::cout << "| MAX_POSITION     = " << std::setw(10) << MAX_POSITION << " |" << std::endl;
    std::cout << "| IPD VMAX         = " << std::setw(10) << INTERVAL_PART_DURATION_VMAX << " |" << std::endl;
    std::cout << "| IPD VMIN         = " << std::setw(10) << INTERVAL_PART_DURATION_VMIN << " |" << std::endl;
    std::cout << "+-------------------------------+" << std::endl;
}


#define _ACCELERATION_DISTANCE_FOR_VELOCITY(VELOCITY) ((VELOCITY) == 0 ? 0 : abs(round((abs(VELOCITY) / VMAX) * STEPS_TO_VMAX)) - TARGET_TOLERANCE)
#define ACCELERATION_DISTANCE_FOR_VELOCITY(VELOCITY) (_ACCELERATION_DISTANCE_FOR_VELOCITY(VELOCITY) > 0 ? _ACCELERATION_DISTANCE_FOR_VELOCITY(VELOCITY) : 0)

class Motor {
public:
    float velocityAtAccelerationStart;
    int isrSinceAccelerationStart;
    int isrForAcceleration;
    int position;
    int target;
    int pwmBit;
    int directionBit;
    State state;
    int index;
    bool direction;
    float velocity;
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

    float calculateDeltaV();

    float deltaV;
};

#endif
