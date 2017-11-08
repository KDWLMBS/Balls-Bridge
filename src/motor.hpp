#ifndef MOTOR_H
#define MOTOR_H



class Motor {
public:
  int position;
  int target;
  int enableBit;
  int directionBit;
  int index;
  Motor(int index);
  void debug();
  void drive(unsigned long *index, int next);
};

#endif
