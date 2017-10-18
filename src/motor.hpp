#ifndef MOTOR_H
#define MOTOR_H

#define MIN_POSITION -1000
#define MAX_POSITION 1000

class Motor {
public:
    unsigned short position;
    unsigned short target;
    unsigned short enableBit;
    unsigned short directionBit;
    unsigned short index;
//public:
  Motor(unsigned int index);
  const void drive(unsigned long *index);
};

#endif
