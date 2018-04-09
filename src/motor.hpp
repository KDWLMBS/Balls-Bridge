#ifndef MOTOR_H
#define MOTOR_H

#include <cstdint>

#define ISR_PER_SECOND 4000

//unit: steps / second
#define VMAX 2000

#define MIN_ISR (ISR_PER_SECOND / VMAX / 2)

//unit: seconds
#define TIME_TO_VMAX 0.25

//steps from center
#define MIN_POSITION (-1000)

//steps from center
#define MAX_POSITION 1000

enum State {
    //DRIVE = drive at max speed in current direction
            DRIVING, ACCELERATING, STOPPING, IDLE
};

class Motor {
public:
    int position;
    int target;
    int pwmBit;
    int directionBit;
    State state;
    int index;
    bool direction;
    int velocity;
    bool isHigh;
    int intervalPartIndex;

    Motor(int index);

    void debug();

    void tick(uint64_t *index);

    void update();

    void drive(uint64_t *index);

    void setTarget(int _target);
};

#endif
