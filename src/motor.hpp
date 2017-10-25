#ifndef MOTOR_H
#define MOTOR_H

#define MIN_POSITION -1000
#define MAX_POSITION 1000

class Motor {
public:
    short position;
    short target;
    unsigned short enableBit;
    unsigned short directionBit;
    unsigned short index;
//public:
  Motor(unsigned int index);
  const void drive(unsigned long *index, int next);
};

#endif
