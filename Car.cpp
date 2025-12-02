#include "Car.hpp"
#include "Crosswalk.h"
#include "Distribution.h"
#include "Trace.h"

using namespace std;


// Constructor
Car::Car(double time, Direction travelDir) {
    this->travelDir = travelDir;
    enterTime = time;

    // calculate speed (ft/s)
    speed = get_uniform(Cross::VJ_A, Cross::VJ_B, autoTraceStream);

};



// GETTERS

double Car::get_enter_time() {
    return enterTime;
}

double Car::get_speed() {
    return speed;
}

Direction Car::get_direction() {
    return travelDir;
}

bool Car::get_stopped() {
    return stopped;
}

// SETTERS

void Car::set_stopped() {
    stopped = true;
}