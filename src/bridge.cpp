#ifndef RPI
#define RPI false
#endif


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
#include <bitset>
#include <list>
#include <vector>
#include <string>
#include "motor.hpp"
#include "controller.hpp"

#if RPI
#include <wiringPi.h>
#include <wiringPiSPI.h>
#define INTERRUPT_PIN 0
#endif //RPI

Controller controller(0, 15);

void interrupt() {
  unsigned long data = 0;
  controller.drive(&data);
  std::vector<unsigned char> byteArr(4);
  for (int i = 0; i < 4; i++) {
    byteArr[3 - i] = (data >> (i * 8));
  }
  unsigned char arr[4];
  std::copy(byteArr.begin(), byteArr.end(), arr);

  #if RPI
  wiringPiSPIDataRW (0, arr, 4);
  #endif //RPI
}

int main() {

  //setup rpi + interruptss
  #if RPI
  if (wiringPiSetup () < 0) {
      //fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
      return 1;
  }

  wiringPiSPISetup (0, 2000000);

  if ( wiringPiISR (INTERRUPT_PIN, INT_EDGE_RISING, &interrupt) < 0 ) {
      //fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
      return 1;
  }
  #endif //RPI

  while(1) {

  }
}
