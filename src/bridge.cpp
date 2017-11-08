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
#include <chrono>
#include <thread>
#include <string>
#include <fstream>

#include <algorithm>
#include <sstream>
#include <functional>
#include <cctype>
#include <locale>

#include "motor.hpp"
#include "controller.hpp"

#if RPI
#include <wiringPi.h>
#include <wiringPiSPI.h>
#define INTERRUPT_PIN 0
#define SPI_FREQUENCY 2000000
#endif //RPI


static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

Controller controller(0, 15);
volatile int next = 0;

void interrupt() {
  unsigned long data = 0;
  controller.drive(&data, next);
  std::vector<unsigned char> byteArr(4);
  for (int i = 0; i < 4; i++) {
    //byteArr[3 - i] = 0xff;
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

  //set SPI frequency
  wiringPiSPISetup (0, SPI_FREQUENCY);

  if ( wiringPiISR (INTERRUPT_PIN, INT_EDGE_RISING, &interrupt) < 0 ) {
      //fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
      return 1;
  }
  #endif //RPI

  while(1) {
    #if RPI
    //if we are on a raspberry we read the ballsfile every 100 ms and set the motor positions accordingly
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    //create a stream with the file
    std::ifstream ifs("/var/ballsfile");

    //convert it to a string
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    //prepare the string
    trim(content);

    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    int index = 0;
    while ((pos = content.find(delimiter)) != std::string::npos) {
        token = content.substr(0, pos);
        content.erase(0, pos + delimiter.length());
        if(index < controller.motors.size()) {
          controller.motors[index].target = stoi(token);
        }
        index++;
    }
    std::cout << controller.motors[0].position << ">" << controller.motors[0].target << std::endl;
    #endif //RPI
  }
}
