#define CATCH_CONFIG_MAIN

#include <cstdint>
#include "catch.hpp"
#include "../src/motor.cpp"

TEST_CASE("Stopping wrong direction up", "wrong direction up") {
    uint64_t data = 0;

    Motor motor(0);
    motor.position = -10;
    motor.target = 200;
    motor.velocity = -10;
    motor.tick(&data);
    REQUIRE(motor.state == State::STOPPING);
}

TEST_CASE("Stopping at destination because too fast", "at destination but too fast") {
    uint64_t data = 0;

    Motor motor(0);
    motor.position = -500;
    motor.target = -500;
    motor.velocity = -100;
    motor.tick(&data);
    REQUIRE(motor.state == State::STOPPING);
}

TEST_CASE("Accelerating 1", "accelerate 1") {
    uint64_t data = 0;

    Motor motor(0);
    motor.position = -10;
    motor.target = 200;
    motor.velocity = 10;
    motor.tick(&data);
    REQUIRE(motor.state == State::ACCELERATING);
}

TEST_CASE("Idle 1", "idle") {
    uint64_t data = 0;

    Motor motor(0);
    motor.position = -10;
    motor.target = -10;
    motor.velocity = 0;
    motor.tick(&data);
    REQUIRE(motor.state == State::IDLE);
}