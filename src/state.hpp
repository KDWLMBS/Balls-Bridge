#ifndef STATE_HPP
#define STATE_HPP

enum class State {
    //DRIVE = drive at max speed in current direction
            DRIVING, ACCELERATING, STOPPING, IDLE
};

inline const char *STATE_TO_STRING(State s) {
    switch (s) {
        case State::DRIVING:
            return "DRIVING";
        case State::ACCELERATING:
            return "ACCELERATING";
        case State::STOPPING:
            return "STOPPING";
        case State::IDLE:
            return "IDLE";
    }
}

#endif //STATE_HPP
