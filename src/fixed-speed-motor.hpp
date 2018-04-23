#ifndef FIXED_SPEED_MOTOR_H
#define FIXED_SPEED_MOTOR_H

#include "state.hpp"

#ifndef ISR_PER_SECOND
#define ISR_PER_SECOND 4000
#endif

#ifndef VELOCITY
#define VELOCITY 100
#endif

#ifndef INTERVAL_PART_DURATION
#define INTERVAL_PART_DURATION (round(ISR_PER_SECOND / VELOCITY / 2))
#endif

class FixedSpeedMotor {
public:
    int intervalPartCounter;
    int intervalPartDuration;

    FixedSpeedMotor(int index);

    void tick(uint64_t *index);

    void update();

    void drive(uint64_t *index);

    State state;
    int target;
    int position;
    int index;
    bool shouldUpdate;
    bool intervalPartIsHigh;
    bool direction;
    int pwmBit;
    int directionBit;
};

#endif //FIXED_SPEED_MOTOR_H
